#ifndef __LIB_FREE_TYPE_H__
#define __LIB_FREE_TYPE_H__

#include "../../common/src/LibCommon.h"

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

