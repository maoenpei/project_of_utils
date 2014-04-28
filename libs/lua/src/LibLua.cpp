

#include "LibLua.h"

class LuaImpl : public Libs::Lua
{
	LIB_IMPL_DESTROY();
};

Libs::Lua * createLuaState()
{
	return new LuaImpl();
}
