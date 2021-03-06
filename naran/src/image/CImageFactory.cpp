

#include "image/CImageFactory.h"
#include "core/CLibLoader.h"
#include "libPNG/src/LibPNG.h"
#include "libJPEG/src/LibJPEG.h"

NS_DEF_NARAN{

	class PNGLoader : public IImageLoader
	{
	public:
		grab(LibLoader) loader;
		Libs::ReadPNG *(*readerCreateFunc)();
		Libs::WritePNG *(*writerCreateFunc)();
		PNGLoader() : loader(LibLoader::create("UseLibPNG")) {
			readerCreateFunc = (Libs::ReadPNG *(*)())loader->getFunction("createReadPNG");
			writerCreateFunc = (Libs::WritePNG *(*)())loader->getFunction("createWritePNG");
		}
		inline grab(Image) readImageData(Libs::ReadPNG *reader)
		{
			grab(Image) ret;
			Libs::PNGImage image;
			reader->getinfo(&image);
			image.buffer = new byte[image.rowbytes * image.height];
			reader->getinfo(&image);
			
			ret = Image::create();
			ret->setAlpha(image.format == Libs::PNGImage_RGBA);
			ret->resize(g2d::Sizei(image.width, image.height), image.buffer);
			return ret;
		}
		virtual grab(Image) loadImage(const char *filename)
		{
			grab(Image) ret;
			Libs::ReadPNG *reader = readerCreateFunc();
			if (reader->openfile(filename)){
				ret = readImageData(reader);
				reader->close();
			}
			reader->destroy();
			return ret;
		}
		virtual grab(Image) loadImage(arr(byte) data)
		{
			grab(Image) ret;
			Libs::ReadPNG *reader = readerCreateFunc();
			if (reader->opendata(data.get(), data.size())){
				ret = readImageData(reader);
				reader->close();
			}
			reader->destroy();
			return ret;
		}
		virtual bool saveImage(const char *filename, grab(Image) img)
		{
			bool ret = false;
			Libs::PNGImage image;
			image.format = (img->getAlpha() ? Libs::PNGImage_RGBA : Libs::PNGImage_RGB);
			image.width = img->getWidth();
			image.height = img->getHeight();
			image.buffer = img->getData();
			Libs::WritePNG *writer = writerCreateFunc();
			if (writer->openfile(filename)){
				writer->inputinfo(&image);
				writer->close();
				ret = true;
			}
			writer->destroy();
			return ret;
		}
	};

	class JPEGLoader : public IImageLoader
	{
	public:
		grab(LibLoader) loader;
		Libs::ReadJPEG *(*readerCreateFunc)();
		Libs::WriteJPEG *(*writerCreateFunc)();
		JPEGLoader() : loader(LibLoader::create("UseLibJpeg")) {
			readerCreateFunc = (Libs::ReadJPEG *(*)())loader->getFunction("createReadJPEG");
			writerCreateFunc = (Libs::WriteJPEG *(*)())loader->getFunction("createWriteJPEG");
		}
		inline grab(Image) readImageData(Libs::ReadJPEG *reader)
		{
			grab(Image) ret;
			Libs::JPEGImage image;
			reader->getinfo(&image);
			image.buffer = new byte[image.width * image.height * 3];
			reader->getinfo(&image);
			
			ret = Image::create();
			ret->setAlpha(false);
			ret->resize(g2d::Sizei(image.width, image.height), image.buffer);
			return ret;
		}
		virtual grab(Image) loadImage(const char *filename)
		{
			grab(Image) ret;
			Libs::ReadJPEG *reader = readerCreateFunc();
			if (reader->openfile(filename)){
				ret = readImageData(reader);
				reader->close();
			}
			reader->destroy();
			return ret;
		}
		virtual grab(Image) loadImage(arr(byte) data)
		{
			grab(Image) ret;
			Libs::ReadJPEG *reader = readerCreateFunc();
			if (reader->opendata(data.get(), data.size())){
				ret = readImageData(reader);
				reader->close();
			}
			reader->destroy();
			return ret;
		}
		virtual bool saveImage(const char *filename, grab(Image) img)
		{
			bool ret = false;
			if (img->getAlpha()){
				return ret;
			}
			Libs::JPEGImage image;
			image.width = img->getWidth();
			image.height = img->getHeight();
			image.buffer = img->getData();
			Libs::WriteJPEG *writer = writerCreateFunc();
			if (writer->openfile(filename)){
				writer->inputinfo(&image);
				writer->close();
				ret = true;
			}
			writer->destroy();
			return ret;
		}
	};

	class DefaultFormatRunnable : public IRunnable
	{
	public:
		void run(){
			/* png support */
			grab(PNGLoader) pngObject(new PNGLoader());
			ImageFactory::shared()->addUserDefinedLoader(
				ImageFormat_PNG, stablize_grab(IImageLoader, PNGLoader, pngObject));
			/* jpeg support */
			grab(JPEGLoader) jpegObject(new JPEGLoader());
			ImageFactory::shared()->addUserDefinedLoader(
				ImageFormat_JPEG, stablize_grab(IImageLoader, JPEGLoader, jpegObject));
		}
	};
	StaticRun<DefaultFormatRunnable> _run;

	grab(Image) ImageFactory::loadFile(c_str filepath, u32 format)
	{
		grab(Image) ret;
		for (int i = 0; i<mLoaders.length(); i++){
			if (format == ImageFormat_Undef || format == mLoaders[i].format){
				ret = mLoaders[i].loader->loadImage(filepath);
				if (ret || format == mLoaders[i].format)
					break;
			}
		}
		return ret;
	}

	grab(Image) ImageFactory::loadData(arr(byte) data, u32 format)
	{
		grab(Image) ret;
		for (int i = 0; i<mLoaders.length(); i++){
			if (format == ImageFormat_Undef || format == mLoaders[i].format){
				ret = mLoaders[i].loader->loadImage(data);
				if (ret || format == mLoaders[i].format)
					break;
			}
		}
		return ret;
	}

	void ImageFactory::saveFile(c_str filepath, u32 format, grab(Image) img)
	{
		for (int i = 0; i<mLoaders.length(); i++){
			if (format == mLoaders[i].format){
				mLoaders[i].loader->saveImage(filepath, img);
				break;
			}
		}
	}

	void ImageFactory::addUserDefinedLoader(u32 format, stable(IImageLoader) loader)
	{
		assert(format != ImageFormat_Undef);
		for (int i = 0; i<mLoaders.length(); i++){
			if (format == mLoaders[i].format){
				return;
			}
		}
		mLoaders.append(ImageLoaderEntry(loader, format), -1);
	}

	CLS_SHARED_DEFAULT(ImageFactory);

}

