#ifndef __NARAN_RUNTIME_H__
#define __NARAN_RUNTIME_H__

#include "core/CommonDef.h"
#include "core/AutoCounter.h"
#include "core/Array.h"
#include "structure/KString.h"

NS_DEF_NARAN{

	class IRunnable
	{
	public: virtual void run() = 0;
	};

	class IInputHandler
	{
	public: virtual void run(arr(char *) args) = 0;
	};

	enum {
		Runtime_Level0 = 0,		// normal
	};

#define CLS_SHARED(CLS)			\
	public: static grab(CLS) shared();

#define CLS_SHARED_DEFAULT(CLS)		\
	grab(CLS) CLS::shared(){static grab(CLS) _obj = new CLS(); return _obj;}

	class CLS_EXPORT Runtime
	{
	public:
		void addRunAtStartup(stable(IRunnable) runnable, int level = Runtime_Level0);
		void startup();

		void addInputHandler(stable(IInputHandler) handler);
		stable(IInputHandler) getInputHandler(c_str name = 0);
		void setCurrentName(c_str name);

	private:
		struct RuntimeEntry
		{
			RuntimeEntry(const stable(IRunnable) &_runnable, int _level) 
				: runnable(_runnable), level(_level){}
			stable(IRunnable) runnable;
			int level;
		};
		Array<RuntimeEntry> mRunnables;
		int searchRuntimeEntry(int level);
		
		struct InputHandlerEntry
		{
			InputHandlerEntry(const String &_name, stable(IInputHandler) _handler)
				: name(_name), handler(_handler){}
			stable(IInputHandler) handler;
			String name;
		};
		Array<InputHandlerEntry> mHandlers;
		String mCurrentName;

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

