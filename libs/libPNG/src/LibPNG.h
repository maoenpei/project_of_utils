#ifndef __LIBS_LIB_PNG_H__
#define __LIBS_LIB_PNG_H__

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
	enum {
		PNGImage_Unknown,
		PNGImage_RGB,
		PNGImage_RGBA,
	};

	struct PNGImage{
		inline PNGImage() : buffer(0){}
		unsigned int format;		// image format
		int rowbytes;				// number of bytes per line
		int width;					// image width
		int height;					// image height
		unsigned char *buffer;		// image buffer
	};

	class ReadPNG
	{
	public:
		LIB_INTERFACES();
		
		virtual bool openfile(const char *filename) = 0;
		virtual bool opendata(const unsigned char *data, size_t size) = 0;
		virtual void close() = 0;

		virtual bool getinfo(PNGImage *info) = 0;
	};

	class PNGWriteFunc
	{
	public:
		virtual void outputdata(const unsigned char *data, size_t size) = 0;
	};

	class WritePNG
	{
	public:
		LIB_INTERFACES();

		virtual bool openfile(const char *filename) = 0;
		virtual bool opendata(PNGWriteFunc *func) = 0;
		virtual void close() = 0;

		virtual bool inputinfo(PNGImage *info) = 0;
	};

};

extern "C" EXPORT_FUNC Libs::ReadPNG * createReadPNG();
extern "C" EXPORT_FUNC Libs::WritePNG * createWritePNG();

#endif

