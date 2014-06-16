#ifndef __LIBS_JPEG_H__
#define __LIBS_JPEG_H__

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
	struct JPEGImage{
		inline JPEGImage() : buffer(0){}
		int width;					// image width
		int height;					// image height
		unsigned char *buffer;		// image buffer
	};
	
	class ReadJPEG
	{
	public:
		LIB_INTERFACES();
		
		virtual bool openfile(const char *filename) = 0;
		virtual bool opendata(const unsigned char *data, size_t size) = 0;
		virtual void close() = 0;

		virtual bool getinfo(JPEGImage *info) = 0;
	};

	class JPEGWriteFunc
	{
	public:
		virtual void outputdata(const unsigned char *data, size_t size) = 0;
	};

	class WriteJPEG
	{
	public:
		LIB_INTERFACES();

		virtual bool openfile(const char *filename) = 0;
		virtual bool opendata(JPEGWriteFunc *func) = 0;
		virtual void close() = 0;

		virtual bool inputinfo(JPEGImage *info) = 0;
	};

};

extern "C" EXPORT_FUNC Libs::ReadJPEG * createReadJPEG();
extern "C" EXPORT_FUNC Libs::WriteJPEG * createWriteJPEG();

#endif

