#ifndef __NARAN_IMAGE_H__
#define __NARAN_IMAGE_H__

#include "AutoReference.h"

namespace Naran {

	typedef unsigned int pxType;

	class Image
	{
		AUTO_PROTOCOL();

	public:
		pxType getPixel(int x, int y);
		void setPixel(int x, int y, pxType px);
		
		pxType *getData();
		int getWidth();
		int getHeight();
		
		void reset();
		void loadFile(const char *filepath);
		void loadFromData(const void *data, size_t size);
		void loadFromBitmap(const pxType *pixels, int w, int h);

		static Auto(Image) create();

	private:
		
	private:
		Image();
		Image(const Image &copy);
		~Image();
	};

};

#endif

