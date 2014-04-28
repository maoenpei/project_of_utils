#ifndef __LIBS_ZLIB_H__
#define __LIBS_ZLIB_H__

#include "../../common/src/LibCommon.h"

namespace Libs
{
	class Zlib
	{
	public:
		LIB_INTERFACES();
		
		virtual bool zcompress(
			unsigned char *dest, 
			int *dstlen, 
			const unsigned char *source, 
			int srclen) = 0;
		
		virtual bool zuncompress(
			unsigned char *dest, 
			int *dstlen, 
			const unsigned char *source, 
			int srclen) = 0;
	};
};

extern "C" EXPORT_FUNC Libs::Zlib * createZlib();

#endif

