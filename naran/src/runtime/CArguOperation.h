#ifndef __NARAN_ARGU_OPERATION_H__
#define __NARAN_ARGU_OPERATION_H__

#include "core/CommonDef.h"
#include "core/AutoCounter.h"
#include "structure/KHashmap.h"

NS_DEF_NARAN{

	class IArguOperator
	{
	public:
		virtual void run() = 0;
		virtual void run(const char *argValue) = 0;
		virtual void run(const char *argValue, const char *argValue2) = 0;
		virtual void run(const char *argValue, const char *argValue2, const char *argValue3) = 0;
	};

	class CLS_EXPORT ArguOperation
	{
	public:
		virtual bool run(arr(char *) args);
		
		void addOperator(const char *argName, stable(IArguOperator) op, int followCount = 1);

		ArguOperation(stable(IArguOperator) defOp);
		
	protected:
		struct ArguTocker
		{
			ArguTocker(const stable(IArguOperator) &_op, int _count) : op(_op), count(_count) {}
			stable(IArguOperator) op;
			int count;
		};
		strMap(grab(ArguTocker)) mArgOps;
		stable(IArguOperator) mDefault;
	};

}

#endif

