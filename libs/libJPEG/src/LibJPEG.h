#ifndef __LIBS_JPEG_H__
#define __LIBS_JPEG_H__

#include "../../common/src/LibCommon.h"

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
		virtual bool openfileptr(FILE *f) = 0;
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

