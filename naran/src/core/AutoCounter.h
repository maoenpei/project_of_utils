#ifndef __NARAN_AUTO_COUNTER_H__
#define __NARAN_AUTO_COUNTER_H__

#include "core/AutoReference.h"

NS_DEF_NARAN{

	class IKeep
	{
	public:
		virtual void keep() = 0;
		virtual void discard() = 0;
	};

	template<typename I>
	class CLS_EXPORT Union_
	{
		I *mInterf;
		IKeep *mObjref;
	public:
		inline Union_() : mInterf(NULL), mObjref(NULL){}
		inline Union_(I *interf, IKeep *objref) 
			: mInterf(interf), mObjref(objref){
			mObjref->keep();
		}
		inline Union_(const Union_ &copy) 
			: mInterf(copy.mInterf), mObjref(copy.mObjref){
			if (mObjref){
				mObjref->keep();
			}
		}
		inline Union_ &operator=(const Union_ &copy){
			if (copy.mObjref){
				copy.mObjref->keep();
			}
			if (mObjref){
				mObjref->discard();
			}
			mInterf = copy.mInterf;
			mObjref = copy.mObjref;
			return *this;
		}
		inline ~Union_(){
			if (mObjref){
				mObjref->discard();
			}
		}
		inline operator bool(){
			return mInterf != NULL;
		}
		inline I *operator->(){
			return mInterf;
		}
	};

	/* auto self-delete object */
#define stable(CLS)		Union_<CLS>

	template<typename T>
	class CLS_EXPORT Keep_ : public IKeep
	{
		T mRef;
		int mReference;
	public:
		inline Keep_(T ref) : mRef(ref), mReference(0) {}
		virtual void keep(){++mReference;}
		virtual void discard(){if (--mReference == 0) delete this;}
	};
// stable of nullptr
#define stablize_null(CLS)					(stable(CLS)())
// stable of ungrabed object
#define stablize_nop(CLS, GCLS, GVAR)		(stable(CLS)((GVAR), new Keep_<GCLS *>(GVAR)))
// stable of grabed object
#define stablize_grab(CLS, GCLS, GVAR)		(stable(CLS)((GVAR).get(), new Keep_<grab(GCLS)>(GVAR)))

}

#endif

