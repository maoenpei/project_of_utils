#ifndef __NARAN_IMAGE_FACTORY_H__
#define __NARAN_IMAGE_FACTORY_H__

#include "core/AutoReference.h"
#include "image/KImage.h"
#include "core/CRuntime.h"
#include "core/Array.h"

NS_DEF_NARAN{

	enum {
		ImageFormat_Undef = -1,
		ImageFormat_PNG = 0,
		ImageFormat_JPEG,
		ImageFormat_UserDef = 0x20
	};

	class IImageLoader
	{
	public:
		virtual grab(Image) loadImage(const char *filename) = 0;
		virtual grab(Image) loadImage(arr(byte) data) = 0;
	};

	class CLS_EXPORT ImageFactory
	{
		struct ImageLoaderEntry
		{
			stable(IImageLoader) loader;
			u32 format;
			ImageLoaderEntry(const stable(IImageLoader) &_loader, u32 _format)
				: loader(_loader), format(_format){}
		};
		Array<ImageLoaderEntry> mLoaders;
	public:
		grab(Image) loadFile(c_str filepath, u32 format = ImageFormat_Undef);
		grab(Image) loadData(arr(byte) data, u32 format = ImageFormat_Undef);

		void addUserDefinedLoader(u32 format, stable(IImageLoader) loader);

		CLS_SHARED(ImageFactory);
	};

}

#endif

