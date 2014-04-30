

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
		PNGLoader() : loader(LibLoader::create("UseLibPNG")) {
			readerCreateFunc = (Libs::ReadPNG *(*)())loader->getFunction("createReadPNG");
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
			ret->resize(image.width, image.height, image.buffer);
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
	};

	class DefaultFormatRunnable : public IRunnable
	{
	public:
		void run(){
			grab(PNGLoader) object(new PNGLoader());
			stable(IImageLoader) loader = stablize(IImageLoader, PNGLoader, object);
			ImageFactory::shared()->addUserDefinedLoader(
				ImageFormat_PNG, loader);
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

}

