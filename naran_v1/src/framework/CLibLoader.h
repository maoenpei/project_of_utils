#ifndef __NARAN_LIB_LOADER_H__
#define __NARAN_LIB_LOADER_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"

NS_DEF_NARAN{
	
	class CLS_EXPORT LibLoader
	{
	// methods
	public:
		bool load(c_str libpath);
		void free();
		
		bool isValid();
		func_ptr getFunction(c_str funcname);

		LibLoader();
		~LibLoader();
		
	// members
	private:
		void * mHandler;
	};

};

#endif

