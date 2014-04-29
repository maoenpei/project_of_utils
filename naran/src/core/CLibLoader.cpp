

#include "CLibLoader.h"

#if defined(WIN32)
#include <windows.h>
#define LIBLOAD(PATH)			(void *)LoadLibraryA(PATH)
#define LIBFREE(LIB)			FreeLibrary((HMODULE)(LIB))
#define LIBFUNC(LIB, FUNC)		GetProcAddress((HMODULE)(LIB), FUNC)
#else
#define LIBLOAD(PATH)			dlopen((PATH), RTLD_NOW|RTLD_GLOBAL)
#define LIBFREE(LIB)			dlclose(LIB)
#define LIBFUNC(LIB, FUNC)		dlsym((LIB), FUNC)
#endif

NS_DEF_NARAN{

	bool LibLoader::load(c_str libpath)
	{
		if (mHandler)
			return false;
		mHandler = LIBLOAD(libpath);
		return mHandler != NULL;
	}

	void LibLoader::free()
	{
		if (mHandler){
			LIBFREE(mHandler);
			mHandler = NULL;
		}
	}

	func_ptr LibLoader::getFunction(c_str funcname)
	{
		if (! mHandler)
			return NULL;
		return (func_ptr)LIBFUNC(mHandler, funcname);
	}

	grab(LibLoader) LibLoader::create()
	{
		return grab(LibLoader)(new LibLoader());
	}

	grab(LibLoader) LibLoader::create(c_str libpath)
	{
		grab(LibLoader) loader(new LibLoader());
		loader->load(libpath);
		return loader;
	}

	LibLoader::LibLoader()
		: mHandler(NULL)
	{}

	LibLoader::~LibLoader()
	{
		if (mHandler)
			free();
	}

}

