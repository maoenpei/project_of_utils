#ifndef __NARAN_IMAGE_FACTORY_H__
#define __NARAN_IMAGE_FACTORY_H__

#include "core/AutoReference.h"
#include "image/KImage.h"

NS_DEF_NARAN{

	class ImageFactory
	{
	public:
		grab(Image) loadFile(c_str filepath);
		grab(Image) loadData(common_ptr data, size_t siz);

		static ImageFactory *sharedFactory();
	};

}

#endif

