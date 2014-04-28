

#include "LuaState.h"

class LuaStateImpl : public Libs::LuaState
{
	LIB_IMPL_DESTROY();
};

Libs::LuaState * createLuaState()
{
	return new LuaStateImpl();
}
