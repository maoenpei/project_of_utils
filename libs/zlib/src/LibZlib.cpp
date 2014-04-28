

#include "LibZlib.h"
#include "zlib.h"

class ZlibImpl : public Libs::Zlib
{
public:
	LIB_IMPL_DESTROY();
	
	ZlibImpl()
	{}
	~ZlibImpl()
	{}
	
	virtual bool zcompress(unsigned char *dest, int *dstlen, const unsigned char *source, int srclen)
	{
		return Z_OK == compress(dest, (uLong *)dstlen, source, (uLong)srclen);
	}
	virtual bool zuncompress(unsigned char *dest, int *dstlen, const unsigned char *source, int srclen)
	{
		return Z_OK == uncompress(dest, (uLong *)dstlen, source, (uLong)srclen);
	}
};

Libs::Zlib * createZlib()
{
	return new ZlibImpl();
}

