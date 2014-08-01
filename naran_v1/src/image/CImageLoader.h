#ifndef __NARAN_IMAGE_LOADER_H__
#define __NARAN_IMAGE_LOADER_H__

#include "CommonDef.h"
#include "autoref/Autoreference.h"
#include "KImage.h"

NS_DEF_NARAN{

	class CLS_EXPORT ImageLoader
	{
	public:
		virtual grab(Image) decodeImage(const char *path) = 0;
		virtual bool encodeImage(const char *path, grab(Image) img) = 0;
		
		static grab(ImageLoader) sharedLoader(const char *extName);
		static void registLoader(const char *extName, grab(ImageLoader) loader);
		
	private:
		CLS_HIDE(ImageLoader);
		
		template<class T>
		friend class LoaderRegister;
	};

	template<class T>
	class CLS_EXPORT LoaderRegister
	{
	public:
		LoaderRegister(const char *extName){
			grab(ImageLoader) loader = new T();
			ImageLoader::registLoader(extName, loader);
		}
	};

}

#endif

