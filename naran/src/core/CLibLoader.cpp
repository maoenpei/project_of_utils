

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

namespace Naran {

	bool LibLoader::load(const char *libpath)
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

	func_ptr LibLoader::getFunction(const char *funcname)
	{
		if (! mHandler)
			return NULL;
		return (func_ptr)LIBFUNC(mHandler, funcname);
	}

	Auto(LibLoader) LibLoader::create()
	{
		return Auto(LibLoader)(new LibLoader());
	}

	Auto(LibLoader) LibLoader::create(const char *libpath)
	{
		Auto(LibLoader) loader(new LibLoader());
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

