#ifndef __NARAN_LIB_LOADER_H__
#define __NARAN_LIB_LOADER_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"

NS_DEF_NARAN{
	
	class LibLoader
	{
	// methods
	public:
		bool load(c_str libpath);
		void free();
		func_ptr getFunction(c_str funcname);
		static grab(LibLoader) create();
		static grab(LibLoader) create(c_str libpath);
		
	// members
	private:
		void * mHandler;

		HIDE_CLS_ALL(LibLoader);
	};

};

#endif

