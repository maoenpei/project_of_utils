#ifndef __LIBS_LUA_H__
#define __LIBS_LUA_H__

#include "../../common/src/LibCommon.h"

namespace Libs
{

	class Lua
	{
	public:
		LIB_INTERFACES();
	};

};

extern "C"{
	EXPORT_FUNC Libs::Lua * createLua();
}

#endif

