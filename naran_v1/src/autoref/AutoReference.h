/***************************************

'Auto_' class is used to count references of an object.
It retains an object while construction and releases that while destruction.

This is an outer-object reference counter technology,
which means you can grab an instance with any form of classes.

To use such type of grabbing, you don't need to implement a class inheriting some base class named 'AutoRef',
and you don't need to call the 'retain' or 'release' method of the class neither.

You can also get another 'grab' operator by defining a 'DeleteOp' template class. Like this:
#define user_grab(CLS)		Auto_<CLS, User_DeleteOp<CLS>, Auto_Ref_<CLS>>

Caution:
By default, you can convert a pointer of class A to an Auto_ instance of A, but you can never do that twice.
Once you converted, it is better to grab it and pass it using Auto_ instance.

***************************************/
#ifndef __NARAN_AUTO_REFERENCE_H__
#define __NARAN_AUTO_REFERENCE_H__

#include "CommonDef.h"

NS_DEF_NARAN{

	/* auto delete object */
	template<typename TT>
	struct CLS_EXPORT Auto_Ref_
	{
		unsigned int ref;
		TT *ptr;
		Auto_Ref_(TT *tt) : ptr(tt), ref(1){}
	};

	template<typename TT>
	class CLS_EXPORT DestroyOp_
	{
	public:
		inline explicit DestroyOp_(TT *t) {delete t;}
	};
	
#define AUTO_EXEC_OP()		\
	if (mRef && --mRef->ref == 0){\
		if (mRef->ptr) OP(mRef->ptr);\
		delete mRef;\
	}

	template<typename T, typename OP, typename REF>
	class CLS_EXPORT Auto_
	{
	protected:
		REF *mRef;
		inline Auto_(REF *ref) : mRef(ref){}
	public:
		/* ptr reference record */
		inline Auto_() : mRef(0){}
		inline /* explict */ Auto_(T *t) : mRef(t ? new REF(t) : 0){}
		inline Auto_(const Auto_ &copy) : mRef(copy.mRef) {if (mRef) mRef->ref++;}
		inline Auto_ &operator =(const Auto_ &another){
			if (another.mRef) another.mRef->ref++;
			AUTO_EXEC_OP();
			mRef = another.mRef;
			return *this;
		}
		inline ~Auto_(){AUTO_EXEC_OP();}
		inline void drop(){
			AUTO_EXEC_OP();
			mRef = NULL;
		}

		/* ptr usage */
		inline T *get() const{return mRef->ptr;}
		inline T *operator ->() const{return mRef->ptr;}
		inline T &operator *() const{return *mRef->ptr;}
		inline T &operator [](int index) const{return mRef->ptr[index];}

		/* ptr convertion */
		inline operator bool() const{return mRef != 0 && mRef->ptr != 0;}
	};

#undef AUTO_EXEC_OP

	/* auto delete array */
	template<typename TT>
	struct CLS_EXPORT Array_Ref_ : public Auto_Ref_<TT>
	{
		int mMax;
		Array_Ref_(TT *tt, int max) : Auto_Ref_(tt), mMax(max){}
	};
	
	template<typename TT>
	class CLS_EXPORT DeleteArrayOp_
	{
	public:
		inline explicit DeleteArrayOp_(TT *t) {delete [] t;}
	};

	template<typename T, typename OP, typename REF>
	class CLS_EXPORT Array_ : public Auto_<T, OP, REF>
	{
	public:
		inline Array_() : Auto_(){}
		//inline Array_(int size) : Auto_(new REF(new T[size] (), size)) {}
		inline Array_(T *t, int size) : Auto_(new REF(t, size)) {}
		inline Array_(const Array_ &copy) : Auto_(copy){}
		inline Array_ &operator=(const Array_ &copy){return (Array_ &)Auto_::operator =(copy);}
		inline void reset(T *t, int size){operator=(Array_(t, size));}
		inline void reset(const Array_ &copy){operator=(copy);}
		
		inline T &operator [](int index) const{assert(index >= 0 && index < mRef->mMax); return Auto_::operator [](index);}
		
		/* array apis */
		inline int count(){return mRef->mMax;}
		inline Array_ Copy() const{
			return Copy(0, mRef->nMax);
		}
		inline Array_ Copy(int start, int len) const{
			if (! mRef || start < 0 || start >= mRef->nMax || len <= 0 || start + len > mRef->nMax){
				return Array_();
			}
			T *t = new T[len]();
			for (int i = 0; i<len; i++){
				t[i] = mRef->ptr[i+start];
			}
			return Array_(t, len);
		}
	};

#define CLS_HIDE(CLS)			protected:CLS();CLS(const CLS&copy);~CLS();friend class DestroyOp_<CLS>;
#define grab(CLS)				Auto_<CLS, DestroyOp_<CLS>, Auto_Ref_<CLS>>
#define nullof(CLS)				((CLS *)0)

#define more(CLS)			Auto_<CLS, DeleteArrayOp_<CLS>, Auto_Ref_<CLS>>
#define arr(CLS)			Array_<CLS, DeleteArrayOp_<CLS>, Array_Ref_<CLS>>

	/* array of grab */
#define arr_grab(CLS)		arr(grab(CLS))

};

#endif

