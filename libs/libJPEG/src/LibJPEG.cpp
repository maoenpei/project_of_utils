

#include "LibJpeg.h"
#include "jpeglib.h"
#include <setjmp.h>

struct libjpeg_err_mgr : public jpeg_error_mgr
{
	jmp_buf jmp_buffer;
};

static void libjpeg_common_exit(j_common_ptr cinfo)
{
	struct libjpeg_err_mgr *err = (struct libjpeg_err_mgr *)cinfo->err;
	longjmp(err->jmp_buffer, 1);
}

class ReadJPEGImpl : public Libs::ReadJPEG
{
public:
	LIB_IMPL_DESTROY();

	ReadJPEGImpl()
		: m_cinfo_ptr(NULL)
		, m_err_ptr(NULL)
		, m_file_ptr(NULL)
	{}
	~ReadJPEGImpl()
	{
		close();
	}

	struct jpeg_decompress_struct *m_cinfo_ptr;
	struct libjpeg_err_mgr *m_err_ptr;
	FILE *m_file_ptr;
	inline void init_cinfo()
	{
		m_cinfo_ptr = new struct jpeg_decompress_struct;
		m_err_ptr = new struct libjpeg_err_mgr;
		m_cinfo_ptr->err = jpeg_std_error(m_err_ptr);
		m_err_ptr->error_exit = libjpeg_common_exit;
		
		jpeg_create_decompress(m_cinfo_ptr);
	}
	inline bool get_cinfo_head()
	{
		struct libjpeg_err_mgr *err = (struct libjpeg_err_mgr *)m_cinfo_ptr->err;
		if (setjmp(err->jmp_buffer)){
			return false;
		}
		jpeg_read_header(m_cinfo_ptr, TRUE);
		if (m_cinfo_ptr->jpeg_color_space == JCS_RGB || 
			m_cinfo_ptr->jpeg_color_space == JCS_GRAYSCALE || m_cinfo_ptr->jpeg_color_space == JCS_YCbCr)
			m_cinfo_ptr->out_color_space = JCS_RGB;
		else
			return false;
		return true;
	}
	virtual bool openfile(const char *filename)
	{
		if (m_cinfo_ptr)
			return false;
		
		m_file_ptr = fopen(filename, "rb");
		if (! m_file_ptr)
			return false;

		init_cinfo();

		jpeg_stdio_src(m_cinfo_ptr, m_file_ptr);
		if (! get_cinfo_head()){
			this->close();
			return false;
		}
		return true;
	}
	virtual bool openfileptr(FILE *f)
	{
		if (m_cinfo_ptr)
			return false;

		init_cinfo();

		jpeg_stdio_src(m_cinfo_ptr, f);
		if (! get_cinfo_head()){
			this->close();
			return false;
		}
		return true;
	}
	virtual bool opendata(const unsigned char *data, size_t size)
	{
		if (m_cinfo_ptr)
			return false;

		init_cinfo();
		
		jpeg_mem_src(m_cinfo_ptr, (unsigned char *)data, size);
		if (! get_cinfo_head()){
			this->close();
			return false;
		}
		return true;
	}
	virtual void close()
	{
		if (m_cinfo_ptr){
			jpeg_destroy_decompress(m_cinfo_ptr);
			delete m_cinfo_ptr;
			m_cinfo_ptr = NULL;
		}
		if (m_err_ptr){
			delete m_err_ptr;
			m_err_ptr = NULL;
		}
		if (m_file_ptr){
			fclose(m_file_ptr);
			m_file_ptr = NULL;
		}
	}

	virtual bool getinfo(Libs::JPEGImage *info)
	{
		if (m_cinfo_ptr == NULL)
			return false;
		info->width = m_cinfo_ptr->output_width;
		info->height = m_cinfo_ptr->output_height;
		if (info->buffer){
			jpeg_start_decompress(m_cinfo_ptr);
			unsigned char *scan = info->buffer;
			while(m_cinfo_ptr->output_scanline < m_cinfo_ptr->output_height){
				jpeg_read_scanlines(m_cinfo_ptr, &scan, 1);
				scan += m_cinfo_ptr->output_width * 3;
			}
			jpeg_finish_decompress(m_cinfo_ptr);
		}
		return true;
	}
};

class WriteJPEGImpl : public Libs::WriteJPEG
{
public:
	LIB_IMPL_DESTROY();
	
	WriteJPEGImpl()
		: m_cinfo_ptr(NULL)
		, m_err_ptr(NULL)
		, m_file_ptr(NULL)
	{}
	~WriteJPEGImpl()
	{
		close();
	}

	struct jpeg_compress_struct *m_cinfo_ptr;
	struct libjpeg_err_mgr *m_err_ptr;
	FILE *m_file_ptr;
	inline void init_cinfo()
	{
		m_cinfo_ptr = new struct jpeg_compress_struct;
		m_err_ptr = new struct libjpeg_err_mgr;
		m_cinfo_ptr->err = jpeg_std_error(m_err_ptr);
		m_err_ptr->error_exit = libjpeg_common_exit;
		
		jpeg_create_compress(m_cinfo_ptr);
	}
	virtual bool openfile(const char *filename)
	{
		if (m_cinfo_ptr)
			return false;

		m_file_ptr = fopen(filename, "wb+");
		if (! m_file_ptr)
			return false;

		init_cinfo();

		jpeg_stdio_dest(m_cinfo_ptr, m_file_ptr);
		return true;
	}
	static void data_to_memory(j_compress_ptr cinfo, void *wr_data, unsigned char *buffer, size_t size)
	{
		Libs::JPEGWriteFunc *func = (Libs::JPEGWriteFunc *)wr_data;
		func->outputdata(buffer, size);
	}
	virtual bool opendata(Libs::JPEGWriteFunc *func)
	{
		if (m_cinfo_ptr)
			return false;

		init_cinfo();

		jpeg_fn_dest(m_cinfo_ptr, data_to_memory, func);
		return true;
	}
	virtual void close()
	{
		if (m_cinfo_ptr){
			jpeg_destroy_compress(m_cinfo_ptr);
			delete m_cinfo_ptr;
			m_cinfo_ptr = NULL;
		}
		if (m_err_ptr){
			delete m_err_ptr;
			m_err_ptr = NULL;
		}
		if (m_file_ptr){
			fclose(m_file_ptr);
			m_file_ptr = NULL;
		}
	}

	virtual bool inputinfo(Libs::JPEGImage *info)
	{
		if (info->width <= 0 || info->height <= 0 || info->buffer == NULL)
			return false;

		m_cinfo_ptr->image_width = info->width;
		m_cinfo_ptr->image_height = info->height;
		m_cinfo_ptr->input_components = 3;
		m_cinfo_ptr->in_color_space = JCS_RGB;

		jpeg_set_defaults(m_cinfo_ptr);

		jpeg_start_compress(m_cinfo_ptr, TRUE);
		unsigned char *scan = info->buffer;
		while(m_cinfo_ptr->next_scanline < m_cinfo_ptr->image_height){
			jpeg_write_scanlines(m_cinfo_ptr, &scan, 1);
			scan += info->width * 3;
		}
		jpeg_finish_compress(m_cinfo_ptr);
		return true;
	}
};

Libs::ReadJPEG * createReadJPEG()
{
	return new ReadJPEGImpl();
}

Libs::WriteJPEG * createWriteJPEG()
{
	return new WriteJPEGImpl();
}

