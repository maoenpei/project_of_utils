#ifndef __NARAN_ARGU_OPERATION_H__
#define __NARAN_ARGU_OPERATION_H__

#include "core/CommonDef.h"
#include "core/AutoCounter.h"
#include "structure/KHashmap.h"

NS_DEF_NARAN{

	class IArguOperator
	{
	public:
		virtual void run(const char *argValue) = 0;
	};

	class CLS_EXPORT ArguOperation
	{
	public:
		virtual bool run(arr(char *) args);
		
		void addOperator(const char *argName, stable(IArguOperator) op);

		ArguOperation(stable(IArguOperator) defOp);
		
	protected:
		strMap(stable(IArguOperator)) mArgOps;
		stable(IArguOperator) mDefault;
	};

}

#endif

