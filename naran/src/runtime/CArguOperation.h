#ifndef __NARAN_ARGU_OPERATION_H__
#define __NARAN_ARGU_OPERATION_H__

#include "core/CommonDef.h"
#include "core/AutoCounter.h"

NS_DEF_NARAN{

	class IArguOperator
	{
	public:
		virtual void run(const char *argValue) = 0;
	};

	class CLS_EXPORT ArguOperation
	{
	public:
		ArguOperation(arr(char *) args);

		void setDefaultOperator(stable(IArguOperator) op);
		void addOperator(const char *argName, stable(IArguOperator) op);

		virtual void run();

	private:
		arr(char *) mArgs;
	};

}

#endif

