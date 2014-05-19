

#include "runtime/CArguOperation.h"


NS_DEF_NARAN{

	void ArguOperation::addOperator(const char *argName, stable(IArguOperator) op)
	{
		mArgOps.set(argName, op);
	}

	bool ArguOperation::run(arr(char *) args)
	{
		for (int i = 0; i<args.size(); i++){
			const char *pName = args[i];
			if (pName[0] == '-'){
				const char *pKey = pName + 1;
				stable(IArguOperator) interf = mArgOps.get(pKey);
				if (interf && i+1 < args.size()){
					const char *pValue = args[++i];
					interf->run(pValue);
				}else{
					printf("unknown argument or argument not enough!\n");
					return false;
				}
			}else{
				if (mDefault){
					mDefault->run(pName);
				}else{
					printf("unknown argument!\n");
					return false;
				}
			}
		}
		return true;
	}

	ArguOperation::ArguOperation(stable(IArguOperator) defOp)
		: mDefault(defOp)
	{
	}

}

