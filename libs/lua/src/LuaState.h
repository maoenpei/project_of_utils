#ifndef __LIB_LUA_STATE_H__
#define __LIB_LUA_STATE_H__

#include "../../common/src/LibCommon.h"

namespace Libs
{

	class LuaState
	{
	public:
		LIB_INTERFACES();
	};

};

extern "C"{
	EXPORT_FUNC Libs::LuaState * createLuaState();
}

#endif

