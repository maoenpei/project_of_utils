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
	class CLS_EXPORT Closure_
	{
		I *mInterf;
		IKeep *mObjref;
	public:
		inline Closure_(I *interf, IKeep *objref) 
			: mInterf(interf), mObjref(objref){
			mObjref->keep();
		}
		inline Closure_(const Closure_ &copy) 
			: mInterf(copy.mInterf), mObjref(copy.mObjref){
			mObjref->keep();
		}
		inline Closure_ &operator=(const Closure_ &copy){
			copy.mObjref->keep();
			mObjref->discard();
			mInterf = copy.mInterf;
			mObjref = copy.mObjref;
			return *this;
		}
		inline ~Closure_(){
			mObjref->discard();
		}
		I *operator->(){
			return mInterf;
		}
	};

	/* auto self-delete object */
#define stable(CLS)		Closure_<CLS>

	/* establish stable object from grab */
	template<typename T>
	class CLS_EXPORT GrabKeep_ : public IKeep
	{
		grab(T) mGrab;
		int mReference;
	public:
		inline GrabKeep_(const grab(T) &grab) : mGrab(grab), mReference(0) {}
		virtual void keep(){++mReference;}
		virtual void discard(){if (--mReference == 0) delete this;}
	};

#define stablize(CLS, GCLS, GVAR)		(stable(CLS)((GVAR).get(), new GrabKeep_<GCLS>(GVAR)))
	
	/* array of stable object */
#define grab_stable(CLS)				grab(stable(CLS))
#define arr_grab_stable(CLS)			arr(grab(stable(CLS)))

}

#endif

