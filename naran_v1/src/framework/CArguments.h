#ifndef __NARAN_ARGUMENTS_H__
#define __NARAN_ARGUMENTS_H__

#include "CommonDef.h"
#include "autoref/AutoCounter.h"
#include "structure/KHashmap.h"
#include "structure/KArray.h"

NS_DEF_NARAN{

	class IArguOperator
	{
	public:
		virtual int type() = 0;
		virtual arr(char *) getStrings() = 0;
	};

	class CLS_EXPORT ArguOperation
	{
	public:
		void reset(arr(char *) args);
		interf(IArguOperator) next();
		
		void addOperator(const char *argName, int followCount, int type);

		static grab(ArguOperation) create(int defType);
		
	private:
		struct ArguTocker
		{
			ArguTocker(int _count, int _type) : count(_count), type(_type) {}
			int count;
			int type;
		};
		StringMap(grab(ArguTocker)) mArgOps;
		Array<interf(IArguOperator)> mInterfs;
		int mDefType;
		ArguOperation(int defType);

		CLS_HIDE(ArguOperation);
	};

}

#endif

