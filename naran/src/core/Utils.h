#ifndef __NARAN_UTILS_H__
#define __NARAN_UTILS_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"

NS_DEF_NARAN{

	class CLS_EXPORT Utils
	{
	public:
		static more(char) copyStr(c_str s);
		static u32 hashStr(c_str s);

		CLS_HIDE(Utils);
	};

}

#endif

