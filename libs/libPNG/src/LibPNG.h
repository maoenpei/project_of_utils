#ifndef __LIBS_LIB_PNG_H__
#define __LIBS_LIB_PNG_H__

#include "../../common/src/LibCommon.h"

namespace Libs
{
	enum {
		PNGImage_Unknown,
		PNGImage_RGB,
		PNGImage_RGBA,
	};

	typedef struct{
		unsigned int format;		// image format
		int channels;				// number of bytes per pixel
		int width;					// image width
		int height;					// image height
		unsigned char *buffer;		// image buffer
	}PNGImage;

	class LibPNG
	{
	public:
		LIB_INTERFACES();
		
		virtual bool openfile(const char *filename) = 0;
		virtual bool opendata(const void *data, size_t size) = 0;
		virtual void close() = 0;

		virtual void getinfo(PNGImage *info) = 0;
	};

	class WritePNG
	{
	public:
		LIB_INTERFACES();

		virtual void openfile(const char *filename) = 0;
		virtual void opencache() = 0;
		virtual void close() = 0;

		virtual bool setinfo(PNGImage *info) = 0;
		virtual size_t getsize() = 0;
		virtual void getbuffer() = 0;
	};

};

extern "C" EXPORT_FUNC Libs::LibPNG * createLibPNG();
extern "C" EXPORT_FUNC Libs::WritePNG * createWritePNG();

#endif

