

#include "LibJpeg.h"
#include "jpeglib.h"

static void libjpeg_common_exit(j_common_ptr cinfo)
{
	throw 1;
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
	struct jpeg_error_mgr *m_err_ptr;
	FILE *m_file_ptr;
	inline bool init_cinfo()
	{
		if (m_cinfo_ptr)
			return false;
		m_cinfo_ptr = new struct jpeg_decompress_struct;
		memset(m_cinfo_ptr, 0, sizeof(struct jpeg_decompress_struct));
		m_err_ptr = new struct jpeg_error_mgr;
		m_cinfo_ptr->err = jpeg_std_error(m_err_ptr);
		m_err_ptr->error_exit = libjpeg_common_exit;
		
		jpeg_create_decompress(m_cinfo_ptr);
		return true;
	}
	virtual bool openfile(const char *filename)
	{
		if (! init_cinfo())
			return false;
		m_file_ptr = fopen(filename, "rb");
		if (! m_file_ptr){
			this->close();
			return false;
		}
		jpeg_stdio_src(m_cinfo_ptr, m_file_ptr);
		jpeg_read_header(m_cinfo_ptr, TRUE);
		return true;
	}
	virtual bool opendata(const unsigned char *data, size_t size)
	{
		if (! init_cinfo())
			return false;
		jpeg_mem_src(m_cinfo_ptr, (unsigned char *)data, size);
		jpeg_read_header(m_cinfo_ptr, TRUE);
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
		jpeg_start_decompress(m_cinfo_ptr);
		info->width = m_cinfo_ptr->output_width;
		info->height = m_cinfo_ptr->output_height;
		if (info->buffer){
			unsigned char *scan = info->buffer;
			while(m_cinfo_ptr->output_scanline < m_cinfo_ptr->output_height){
				jpeg_read_scanlines(m_cinfo_ptr, &scan, 1);
			}
		}
		return true;
	}
};

