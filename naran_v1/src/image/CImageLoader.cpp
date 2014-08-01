

#include "CImageLoader.h"
#include "structure/KHashmap.h"

NS_DEF_NARAN{

	static StringMap(grab(ImageLoader)) &getLoaders(){
		static StringMap(grab(ImageLoader)) s_Loaders;
		return s_Loaders;
	}

	grab(ImageLoader) ImageLoader::sharedLoader(const char *extName)
	{
		return getLoaders().get(extName);
	}

	void ImageLoader::registLoader(const char *extName, grab(ImageLoader) loader)
	{
		getLoaders().set(extName, loader);
	}

	ImageLoader::ImageLoader()
	{}

	ImageLoader::~ImageLoader()
	{}

}


