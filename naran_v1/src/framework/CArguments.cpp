

#include "CArguments.h"

NS_DEF_NARAN{

	class ArguOperatorImpl : public IArguOperator
	{
	public:
		virtual int type()
		{
			return mType;
		}
		virtual arr(char *) getStrings()
		{
			return mArgs;
		}
		arr(char *) mArgs;
		int mType;
	};

	void ArguOperation::reset(arr(char *) args)
	{
		mInterfs.reset();
		int count = args.count();
		for (int i = 0; i<count; i++){
			grab(ArguTocker) tocker = mArgOps.get(args[i]);
			ArguOperatorImpl *impl = new ArguOperatorImpl();
			if (tocker){
				impl->mArgs.reset(new char *[tocker->count](), tocker->count);
				for (int k = 0; k<tocker->count && i+1<count; k++){
					i++;
					impl->mArgs[k] = args[i];
				}
				impl->mType = tocker->type;
			}else{
				impl->mType = mDefType;
				impl->mArgs.reset(new char *[1](), 1);
				impl->mArgs[0] = args[i];
			}
			mInterfs.insert(interf_make_grab(IArguOperator)(impl));
		}
	}

	interf(IArguOperator) ArguOperation::next()
	{
		if (mInterfs.count() > 0){
			interf(IArguOperator) first = mInterfs[0];
			mInterfs.remove(0);
			return first;
		}
		return NULL;
	}

	void ArguOperation::addOperator(const char *argName, int followCount, int type)
	{
		mArgOps.set(argName, new ArguTocker(followCount, type));
	}

	grab(ArguOperation) ArguOperation::create(int defType)
	{
		return new ArguOperation(defType);
	}

	ArguOperation::ArguOperation(int defType)
		: mDefType(defType)
	{}

	ArguOperation::~ArguOperation()
	{}

	
}


