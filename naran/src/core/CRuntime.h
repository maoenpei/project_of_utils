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

	class IInputHandler
	{
	public: virtual void run(int argc, char **argv) = 0;
	};

	enum {
		Runtime_Level0 = 0,		// normal
	};

#define CLS_SHARED(CLS)			\
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
	public:
		void addRunAtStartup(stable(IRunnable) runnable, int level = Runtime_Level0);
		void startup();

		void setInputHandler(stable(IInputHandler) handler);
		stable(IInputHandler) getInputHandler();

	private:
		Array<RuntimeEntry> mRunnables;
		int binaryIndex(int level);
		stable(IInputHandler) mHandler;

		Runtime();

		CLS_SHARED(Runtime);
	};

	template<typename T>
	class StaticRun
	{
	public:
		StaticRun(int level = Runtime_Level0){
			grab(T) object = new T();
			Runtime::shared()->addRunAtStartup(
				stablize_grab(IRunnable, T, object), level);
		}
	};

}

#endif

