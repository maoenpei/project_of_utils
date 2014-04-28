

#include "LibPNG.h"
#include "png.h"
#include "pngpriv.h"

class LibPNGImpl : public Libs::LibPNG
{
public:
	LIB_IMPL_DESTROY();
	
	LibPNGImpl()
	{
	}
	~LibPNGImpl()
	{
		close();
	}

	png_structp m_png_ptr;
	png_infop m_info_ptr;
	
	FILE *m_file_ptr;
	virtual bool openfile(const char *filename)
	{
		if (m_png_ptr)
			return false;
		FILE *f = fopen(filename, "rb");
		if (f){
			unsigned char head[8];
			fread(head, 1, 8, f);
			if (png_check_sig(head, 8)){
				m_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
				m_info_ptr = png_create_info_struct(m_png_ptr);
				png_init_io(m_png_ptr, f);
				png_set_sig_bytes(m_png_ptr, 8);
				png_read_info(m_png_ptr, m_info_ptr);
				m_file_ptr = f;
				return true;
			}
			fclose(f);
		}
		return false;
	}

	typedef struct {
		const unsigned char *pdata;
		size_t size;
	}PNGData;
	PNGData *m_data_ptr;
	static void data_from_memory(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		PNGData *pdata = (PNGData *)png_get_io_ptr(png_ptr);
		if (length <= pdata->size){
			memcpy(data, pdata->pdata, length);
			pdata->pdata += length;
			pdata->size -= length;
		}else{
			memcpy(data, pdata->pdata, pdata->size);
			memset(data + pdata->size, 0, length - pdata->size);
			pdata->pdata += pdata->size;
			pdata->size -= pdata->size;
		}
	}
	virtual bool opendata(const void *data, size_t size)
	{
		if (m_png_ptr)
			return false;
		if (size < 8)
			return false;
		if (png_check_sig((png_const_bytep)data, 8)){
			m_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			m_info_ptr = png_create_info_struct(m_png_ptr);
			m_data_ptr = new PNGData;
			m_data_ptr->pdata = (const unsigned char *)data;
			m_data_ptr->size = size;
			png_set_read_fn(m_png_ptr, m_data_ptr, data_from_memory);
			png_read_info(m_png_ptr, m_info_ptr);
			return true;
		}
		return false;
	}
	virtual void close()
	{
		if (m_png_ptr)
			png_destroy_read_struct(&m_png_ptr, &m_info_ptr, 0);
		if (m_file_ptr){
			fclose(m_file_ptr);
			m_file_ptr = NULL;
		}
		if (m_data_ptr){
			delete m_data_ptr;
			m_data_ptr = NULL;
		}
	}

	virtual void getinfo(Libs::PNGImage *info)
	{
		if (m_info_ptr == NULL || 
			(m_info_ptr->color_type != PNG_COLOR_TYPE_RGB && m_info_ptr->color_type != PNG_COLOR_TYPE_RGB_ALPHA)){
			info->format = Libs::PNGImage_Unknown;
			info->width = 0;
			info->height = 0;
			return;
		}
		info->format = (m_info_ptr->color_type == PNG_COLOR_TYPE_RGB ? Libs::PNGImage_RGB : Libs::PNGImage_RGBA);
		info->channels = m_info_ptr->channels;
		info->width = m_info_ptr->width;
		info->height = m_info_ptr->height;
		if (info->buffer){
			png_bytep *line_arr = new png_bytep[m_info_ptr->height];
			for (int i = 0; i<(int)m_info_ptr->height; i++){
				line_arr[i] = info->buffer + i * m_info_ptr->rowbytes;
			}
			png_read_image(m_png_ptr, line_arr);
			png_read_end(m_png_ptr, NULL);
			delete [] line_arr;
		}
	}
};

Libs::LibPNG * createLibPNG()
{
	return new LibPNGImpl();
}

