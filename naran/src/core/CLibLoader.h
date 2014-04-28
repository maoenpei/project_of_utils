#ifndef __NARAN_LIB_LOADER_H__
#define __NARAN_LIB_LOADER_H__

#include "AutoReference.h"

namespace Naran{

	typedef void (* func_ptr)(void);
	
	class LibLoader
	{
		AUTO_PROTOCOL();
	// methods
	public:
		bool load(const char *libpath);
		void free();
		func_ptr getFunction(const char *funcname);
		static Auto(LibLoader) create();
		static Auto(LibLoader) create(const char *libpath);
		
	// members
	private:
		void * mHandler;

	// forbid
	private:
		LibLoader();
		LibLoader(const LibLoader &copy);
		~LibLoader();
	};

};

#endif

