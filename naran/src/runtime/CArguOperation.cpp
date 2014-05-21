

#include "runtime/CArguOperation.h"


NS_DEF_NARAN{

	void ArguOperation::addOperator(const char *argName, stable(IArguOperator) op, int followCount)
	{
		assert(followCount >= 0 && followCount <= 3);
		mArgOps.set(argName, new ArguTocker(op, followCount));
	}

	bool ArguOperation::run(arr(char *) args)
	{
		for (int i = 0; i<args.size(); i++){
			const char *pName = args[i];
			grab(ArguTocker) tocker = mArgOps.get(pName);
			if (tocker){
				if (i + tocker->count < args.size()){
					stable(IArguOperator) interf = tocker->op;
					const char *pValue = (tocker->count >= 1 ? 0 : args[++i]);
					const char *pValue2 = (tocker->count >= 2 ? 0 : args[++i]);
					const char *pValue3 = (tocker->count >= 3 ? 0 : args[++i]);
					switch(tocker->count){
						case 0:
							interf->run();
							break;
						case 1:
							interf->run(pValue);
							break;
						case 2:
							interf->run(pValue, pValue2);
							break;
						case 3:
							interf->run(pValue, pValue2, pValue3);
							break;
					}
				}else{
					printf("argument not enough!\n");
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

