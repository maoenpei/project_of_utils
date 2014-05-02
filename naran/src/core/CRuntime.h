#ifndef __NARAN_RUNTIME_H__
#define __NARAN_RUNTIME_H__

#include "core/CommonDef.h"
#include "core/AutoCounter.h"
#include "core/Array.h"

NS_DEF_NARAN{

	class IRunnable
	{
	public: virtual void run() = 0;
	};

#define RT_SHARED(CLS)			\
	public: static CLS *shared(){static CLS object; return &object;}

	class CLS_EXPORT Runtime
	{
		struct RuntimeEntry
		{
			RuntimeEntry(const stable(IRunnable) &_runnable, int _level) 
				: runnable(_runnable), level(_level){}
			stable(IRunnable) runnable;
			int level;
		};
		Array<RuntimeEntry> mRunnables;
		int binaryIndex(int level);
		Runtime();
	public:
		void runAtStartup(const stable(IRunnable) runnable, int level);
		
		void startup();
		
		RT_SHARED(Runtime);
	};

	template<typename T>
	class StaticRun
	{
	public:
		StaticRun(int level = 0){
			grab(T) object = new T();
			Runtime::shared()->runAtStartup(stablize(IRunnable, T, object), level);
		}
	};

}

#endif

