

#include "CRuntime.h"

NS_DEF_NARAN{

	int Runtime::binaryIndex(int level)
	{
		int start = 0, end = mCount;
		int mid;
		while(start < end){
			mid = (start + end) / 2;
			if (level > mRunnables[mid]->level)
				start = mid + 1;
			else
				end = mid;
		}
		return start;
	}

	Runtime::Runtime()
		: mCount(0)
		, mMax(4)
	{
		mRunnables = new grab(RuntimeEntry)[mMax];
	}

	void Runtime::runAtStartup(stable(IRunnable) runnable, int level)
	{
		if (mCount == mMax){
			grab(RuntimeEntry) *new_ptr = new grab(RuntimeEntry)[mMax * 2];
			mRunnables.swap(new_ptr, mCount);
			mRunnables = new_ptr;
			mMax *= 2;
		}
		int index = binaryIndex(level);
		mRunnables.displace(index, mCount + 1);
		mRunnables[index] = new RuntimeEntry(runnable, level);
		mCount++;
	}

	void Runtime::startup()
	{
		for (int i = 0; i<mCount; i++){
			mRunnables[i]->runnable->run();
		}
	}

}

