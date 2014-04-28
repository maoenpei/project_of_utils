

#include "LibJpeg.h"
#include "jpeglib.h"


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
		
		jpeg_create_decompress(m_cinfo_ptr);
		return true;
	}
	virtual bool openfile(const char *filename)
	{
		return true;
	}
	virtual bool opendata(const unsigned char *data, size_t size)
	{
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
	}

	virtual bool getinfo(Libs::JPEGImage *info)
	{
		return true;
	}
};

