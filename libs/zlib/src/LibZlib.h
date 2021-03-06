#ifndef __LIBS_ZLIB_H__
#define __LIBS_ZLIB_H__

#include <string>
#include <vector>
#include <list>
#include <map>
#include <stdio.h>

#ifdef WIN32
 #pragma warning(disable:4996)
#endif

#ifndef EXPORT_FUNC
 #ifdef WIN32
 #define EXPORT_FUNC		__declspec(dllexport)
 #else
 #define EXPORT_FUNC		
 #endif
#endif

#ifndef LIB_IMPL_DESTROY
 #define LIB_IMPL_DESTROY()		\
	virtual void destroy(){delete this;}
#endif

#ifndef LIB_INTERFACES
 #define LIB_INTERFACES()		\
	virtual void destroy() = 0;
#endif

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

