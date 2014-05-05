

#include "CRuntime.h"

NS_DEF_NARAN{

	int Runtime::binaryIndex(int level)
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
		int index = binaryIndex(level);
		mRunnables.append(RuntimeEntry(runnable, level), index);
	}

	void Runtime::startup()
	{
		for (int i = 0; i<mRunnables.length(); i++){
			mRunnables[i].runnable->run();
		}
	}

	void Runtime::setInputHandler(stable(IInputHandler) handler)
	{
		mHandler = handler;
	}

	stable(IInputHandler) Runtime::getInputHandler()
	{
		return mHandler;
	}

	Runtime::Runtime()
		: mHandler(stablize_null(IInputHandler))
	{}

}

