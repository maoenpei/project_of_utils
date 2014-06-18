/***************************************

'Union_' class is a union of interfaces and grabbers.
Interface indicates what this class used for, and grabber reference the real instance.

When I get an interface or an abstract object, I don't know what the object is.
So I cannot use template class to define different unions because the interfaces are all the same.

***************************************/
#ifndef __NARAN_AUTO_COUNTER_H__
#define __NARAN_AUTO_COUNTER_H__

#include "autoref/AutoReference.h"

NS_DEF_NARAN{

	class IKeep_
	{
	public:
		virtual void keep() = 0;
		virtual void discard() = 0;
	};

	template<typename I>
	class CLS_EXPORT Union_
	{
		I *mInterf;
		IKeep_ *mObjref;
	public:
		inline Union_() : mInterf(NULL), mObjref(NULL){}
		inline Union_(I *interf, IKeep_ *objref = NULL) 
			: mInterf(interf), mObjref(objref){
			if (mObjref){
				mObjref->keep();
			}
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
#define conn(CLS)		Union_<CLS>

	template<typename T>
	class CLS_EXPORT Keep_ : public IKeep_
	{
		T mRef;
		int mReference;
	public:
		inline Keep_(T ref) : mRef(ref), mReference(0) {}
		virtual void keep(){++mReference;}
		virtual void discard(){if (--mReference == 0) delete this;}
	};
	
	template<class I, class T>
	static inline conn(I) convertGrab(T *object)
	{
		grab(T) inst = object;
		return conn(I)(object, new Keep_<grab(T)>(inst));
	}
	
	// raw interface
#define connect_nop(I)			(conn(I))//(stable(I)((GVAR), new Keep_<GCLS *>(GVAR)))
	// grab of an instance
#define connect_grab(I)			(convertGrab<I>)//(stable(CLS)((GVAR).get(), new Keep_<grab(GCLS)>(GVAR)))

}

#endif

