#ifndef __NARAN_UTILS_H__
#define __NARAN_UTILS_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"

NS_DEF_NARAN{

	class CLS_EXPORT Utils
	{
	public:
		static more(char) copyStr(c_str s);
		static u32 hashStr(c_str s);
		static u32 largeMultiMod(u32 val, u32 coef, u32 mod);
		static int tickMilli();

		CLS_HIDE(Utils);
	};

}

#endif

