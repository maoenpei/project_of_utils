

#include "LibPNG.h"
#include "png.h"
#include "pngpriv.h"

class ReadPNGImpl : public Libs::ReadPNG
{
public:
	LIB_IMPL_DESTROY();
	
	ReadPNGImpl()
		: m_png_ptr(NULL)
		, m_info_ptr(NULL)
		, m_file_ptr(NULL)
		, m_data_ptr(NULL)
	{
	}
	~ReadPNGImpl()
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

	virtual bool openfileptr(FILE *f)
	{
		if (m_png_ptr)
			return false;
		unsigned char head[8];
		fread(head, 1, 8, f);
		if (png_check_sig(head, 8)){
			m_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			m_info_ptr = png_create_info_struct(m_png_ptr);
			png_init_io(m_png_ptr, f);
			png_set_sig_bytes(m_png_ptr, 8);
			png_read_info(m_png_ptr, m_info_ptr);
			return true;
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
	virtual bool opendata(const unsigned char *data, size_t size)
	{
		if (m_png_ptr)
			return false;
		if (size < 8)
			return false;
		if (png_check_sig(data, 8)){
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

	virtual bool getinfo(Libs::PNGImage *info)
	{
		if (m_info_ptr == NULL || 
			(m_info_ptr->color_type != PNG_COLOR_TYPE_RGB && m_info_ptr->color_type != PNG_COLOR_TYPE_RGB_ALPHA)){
			info->format = Libs::PNGImage_Unknown;
			info->width = 0;
			info->height = 0;
			return false;
		}
		info->format = (m_info_ptr->color_type == PNG_COLOR_TYPE_RGB ? Libs::PNGImage_RGB : Libs::PNGImage_RGBA);
		info->rowbytes = m_info_ptr->rowbytes;
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
		return true;
	}
};

class WritePNGImpl : public Libs::WritePNG
{
public:
	LIB_IMPL_DESTROY();
	
	WritePNGImpl()
		: m_png_ptr(NULL)
		, m_info_ptr(NULL)
		, m_file_ptr(NULL)
	{
	}
	~WritePNGImpl()
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
		FILE *f = fopen(filename, "wb+");
		if (f){
			m_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			m_info_ptr = png_create_info_struct(m_png_ptr);
			png_init_io(m_png_ptr, f);
			m_file_ptr = f;
			return true;
		}
		return false;
	}

	static void data_to_memory(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		Libs::PNGWriteFunc *func = (Libs::PNGWriteFunc *)png_get_io_ptr(png_ptr);
		func->outputdata(data, length);
	}
	virtual bool opendata(Libs::PNGWriteFunc *func)
	{
		if (m_png_ptr)
			return false;
		m_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		m_info_ptr = png_create_info_struct(m_png_ptr);
		png_set_write_fn(m_png_ptr, func, data_to_memory, 0);
		return true;
	}
	virtual void close()
	{
		if (m_png_ptr)
			png_destroy_write_struct(&m_png_ptr, &m_info_ptr);
		if (m_file_ptr){
			fclose(m_file_ptr);
			m_file_ptr = NULL;
		}
	}

	virtual bool inputinfo(Libs::PNGImage *info)
	{
		if ((info->format != Libs::PNGImage_RGB && info->format != Libs::PNGImage_RGBA)
			|| info->buffer == NULL || info->width <= 0 || info->height <= 0){
			return false;
		}
		int color_type = (info->format == Libs::PNGImage_RGB ? 
			PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGB_ALPHA);
		png_set_IHDR(m_png_ptr, m_info_ptr, info->width, info->height, 8, 
			color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_write_info(m_png_ptr, m_info_ptr);
		
		int rowbytes = (info->format == Libs::PNGImage_RGB ? 3 : 4) * info->width;
		png_bytep * line_arr = new png_bytep[info->height];
		for (int i = 0; i<info->height; i++){
			line_arr[i] = info->buffer + i * rowbytes;
		}
		png_write_image(m_png_ptr, line_arr);
		png_write_end(m_png_ptr, NULL);
		delete [] line_arr;
		return true;
	}
};

Libs::ReadPNG * createReadPNG()
{
	return new ReadPNGImpl();
}

Libs::WritePNG *createWritePNG()
{
	return new WritePNGImpl();
}

