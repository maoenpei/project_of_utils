#ifndef __LIB_FREE_TYPE_H__
#define __LIB_FREE_TYPE_H__

#ifndef __LIB_COMMON_DEF__

#ifdef WIN32
#pragma warning(disable:4996)
#endif

#ifdef WIN32
#define EXPORT_FUNC		__declspec(dllexport)
#else
#define EXPORT_FUNC		
#endif

#include <string>
#include <vector>
#include <list>
#include <map>
#include <stdio.h>

#define LIB_IMPL_DESTROY()		\
	virtual void destroy(){delete this;}

#define LIB_INTERFACES()		\
	virtual void destroy() = 0;

#endif

namespace Libs
{
	typedef struct {
		int fontSize;		// font size
		int wSize;			// width size
		int hSize;			// height size
		int hAdvance;		// height total size
		int hBaseSize;		// content bottom to base line
		int hBaseAdvance;	// total bottom to base line
	}FontMetrics;

	typedef struct {
		int width;					// bmp width
		int height;					// bmp height
		unsigned char *buffer;		// bmp buffer with width*height bytes alloced memory
	}FontBitmap;

	class FreeType
	{
	public:
		LIB_INTERFACES();

		virtual void LoadLibFile(const char *pathname) = 0;
		virtual void LoadLibMemory(const char *pathname) = 0;
		virtual void FreeLib() = 0;

		virtual void getFontMetrics(unsigned long unicode, int fontSize, FontMetrics *metrics) = 0;
		virtual void RenderFont(unsigned long unicode, int fontSize, FontBitmap *bmp) = 0;
	};
};

extern "C" EXPORT_FUNC Libs::FreeType * createFreeType();

#endif

