

#include "CImageLoader.h"
#include "jpeglib.h"

NS_DEF_NARAN{

	class ImageLoaderJPEG : public ImageLoader
	{
	public:
		virtual grab(Image) decodeImage(const char *path)
		{
			grab(Image) img(nullof(Image));
			FILE *f = fopen(path, "rb");
			if (f){
				struct jpeg_decompress_struct cinfo = {0};
				struct jpeg_error_mgr jerr;
				cinfo.err = jpeg_std_error(&jerr);
				jpeg_create_decompress(&cinfo);
				jpeg_stdio_src(&cinfo, f);
				jpeg_read_header(&cinfo, TRUE);
				if (cinfo.jpeg_color_space == JCS_RGB 
					|| cinfo.jpeg_color_space == JCS_GRAYSCALE 
					|| cinfo.jpeg_color_space == JCS_YCbCr){
					cinfo.out_color_space = JCS_RGB;
				}
				
				jpeg_start_decompress(&cinfo);
				if (cinfo.output_components == 3){
					int channel = cinfo.output_components;
					g2d::Sizei size(cinfo.image_width, cinfo.image_height);
					more(byte) data = new byte[channel * (size.w * size.h)];
					
					JSAMPROW row_pointer[1] = {0};
					while(cinfo.output_scanline < cinfo.image_height){
						row_pointer[0] = data.get() + cinfo.output_scanline * channel * size.w;
						jpeg_read_scanlines(&cinfo, row_pointer, 1);
					}

					img = Image::create(data, size, channel);
				}
				jpeg_finish_decompress(&cinfo);
				
				jpeg_destroy_decompress(&cinfo);
				fclose(f);
			}
			return img;
		}
		
		virtual bool encodeImage(const char *path, grab(Image) img)
		{
			if (!img || !img->isValid()){
				return false;
			}
			more(byte) data = img->getData();
			const g2d::Sizei &size = img->getSize();
			int channel = img->getChannel();
			if (channel != 3){
				return false;
			}
			FILE *f = fopen(path, "wb+");
			if (f){
				struct jpeg_compress_struct cinfo = {0};
				struct jpeg_error_mgr jerr;
				cinfo.err = jpeg_std_error(&jerr);
				jpeg_create_compress(&cinfo);
				jpeg_stdio_dest(&cinfo, f);
				
				cinfo.image_width = size.w;
				cinfo.image_height = size.h;
				cinfo.input_components = 3;
				cinfo.in_color_space = JCS_RGB;
				jpeg_set_defaults(&cinfo);

				jpeg_start_compress(&cinfo, TRUE);
				JSAMPROW row_pointer[1] = {0};
				while(cinfo.next_scanline < cinfo.image_height){
					row_pointer[0] = data.get() + cinfo.next_scanline * channel * size.w;
					jpeg_write_scanlines(&cinfo, row_pointer, 1);
				}
				jpeg_finish_compress(&cinfo);
				
				jpeg_destroy_compress(&cinfo);
				fclose(f);
				return true;
			}
			return false;
		}
	};

	static LoaderRegister<ImageLoaderJPEG> _inst(".jpg");

}

