

#include "LibJpeg.h"
#include "jpeglib.h"


class ReadJPEGImpl : public Libs::ReadJPEG
{
public:
	LIB_IMPL_DESTROY();

	ReadJPEGImpl()
	{}
	~ReadJPEGImpl()
	{}

	
};

