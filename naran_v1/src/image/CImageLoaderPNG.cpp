

#include "CImageLoader.h"
#include "png.h"
#include "pngpriv.h"
#include <stdio.h>

NS_DEF_NARAN{

#define SIG_HEAD_L		(8)

	class ImageLoaderPNG : public ImageLoader
	{
	public:
		virtual grab(Image) decodeImage(const char *path)
		{
			grab(Image) img(nullof(Image));
			FILE *f = fopen(path, "rb");
			if (f){
				unsigned char head[SIG_HEAD_L];
				fread(head, 1, SIG_HEAD_L, f);
				if (png_check_sig(head, SIG_HEAD_L)){
					png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
					png_infop info_ptr = png_create_info_struct(png_ptr);
					png_init_io(png_ptr, f);
					png_set_sig_bytes(png_ptr, SIG_HEAD_L);
					png_read_info(png_ptr, info_ptr);
					if (info_ptr->color_type == PNG_COLOR_TYPE_RGB
						|| info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA){
						int channel = (info_ptr->color_type == PNG_COLOR_TYPE_RGB ? 3 : 4);
						g2d::Sizei size(info_ptr->width, info_ptr->height);
						more(byte) data = new byte[channel * (size.w * size.h)];
						
						png_bytep *line_arr = new png_bytep[info_ptr->height];
						for (int i = 0; i<(int)info_ptr->height; i++){
							line_arr[i] = data.get() + i * channel * info_ptr->width;
						}
						png_read_image(png_ptr, line_arr);
						png_read_end(png_ptr, NULL);
						delete [] line_arr;

						img = Image::create(data, size, channel);
					}
					png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
				}
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
			if (channel != 3 && channel != 4){
				return false;
			}
			FILE *f = fopen(path, "wb+");
			if (f){
				png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
				png_infop info_ptr = png_create_info_struct(png_ptr);
				png_init_io(png_ptr, f);
				int color_type = (channel == 3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGB_ALPHA);
				png_set_IHDR(png_ptr, info_ptr, size.w, size.h, 8, 
					color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
				png_write_info(png_ptr, info_ptr);
				
				png_bytep * line_arr = new png_bytep[size.h];
				for (int i = 0; i<size.h; i++){
					line_arr[i] = data.get() + i * channel * size.w;
				}
				png_write_image(png_ptr, line_arr);
				png_write_end(png_ptr, NULL);
				delete [] line_arr;
				fclose(f);
				return true;
			}
			return false;
		}
		
	};

	static LoaderRegister<ImageLoaderPNG> _inst(".png");

	
}

