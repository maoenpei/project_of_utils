

#include "runtime/CRuntime.h"

NS_DEF_NARAN{

	int Runtime::searchRuntimeEntry(int level)
	{
		int start = 0, end = mRunnables.length();
		int mid;
		while(start < end){
			mid = (start + end) / 2;
			if (level > mRunnables[mid].level)
				start = mid + 1;
			else
				end = mid;
		}
		return start;
	}

	void Runtime::addRunAtStartup(stable(IRunnable) runnable, int level)
	{
		int index = searchRuntimeEntry(level);
		mRunnables.append(RuntimeEntry(runnable, level), index);
	}

	void Runtime::startup()
	{
		for (int i = 0; i<mRunnables.length(); i++){
			mRunnables[i].runnable->run();
		}
	}

	void Runtime::addInputHandler(stable(IInputHandler) handler)
	{
		mHandlers.append(InputHandlerEntry(mCurrentName, handler));
	}

	stable(IInputHandler) Runtime::getInputHandler(c_str name)
	{
		String sName(name);
		for (int i = 0; i<mHandlers.length(); i++){
			if (! sName || sName == mHandlers[i].name){
				return mHandlers[i].handler;
			}
		}
		return stablize_null(IInputHandler);
	}

	void Runtime::setCurrentName(c_str name)
	{
		mCurrentName = name;
	}

	Runtime::Runtime()
	{}

	CLS_SHARED_DEFAULT(Runtime);

}

