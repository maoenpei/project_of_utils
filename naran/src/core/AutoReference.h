#ifndef __NARAN_AUTO_REFERENCE_H__
#define __NARAN_AUTO_REFERENCE_H__

#include "core/CommonDef.h"

NS_DEF_NARAN{

	/* auto delete object */
	template<typename TT>
	struct Auto_Ref_
	{
		unsigned int ref;
		TT *ptr;
		Auto_Ref_(TT *tt) : ptr(tt), ref(1){}
	};

	template<typename TT>
	class DestroyOp_
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
	class Auto_
	{
	protected:
		REF *mRef;
	public:
		/* ptr reference record */
		inline Auto_() : mRef(0){}
		inline /* explict */ Auto_(T *t) : mRef(new REF(t)){}
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
		inline T *get(){return mRef->ptr;}
		inline T *operator ->(){return mRef->ptr;}
		inline T &operator *(){return *mRef->ptr;}
		inline T &operator[](int index){return mRef->ptr[index];}

		/* ptr convertion */
		inline operator bool(){return mRef != 0 && mRef->ptr != 0;}
		template<class U>
		inline operator U * (){return (U *)mRef->ptr;}
	};

#undef AUTO_EXEC_OP

//#define AUTO_PROTOCOL()			virtual void destroy(){delete this;}
#define HIDE_CLS_ALL(CLS)		protected:CLS();CLS(const CLS&copy);~CLS();friend class DestroyOp_<CLS>;
#define grab(CLS)				Auto_<CLS, DestroyOp_<CLS>, Auto_Ref_<CLS>>
#define g(CLS)					grab(CLS)

	/* auto delete array */
	template<typename TT>
	struct Array_Ref_ : public Auto_Ref_<TT>
	{
		int mMax;
		Array_Ref_(TT *tt) : Auto_Ref_(tt), mMax(4){}
		Array_Ref_(TT *tt, int max) : Auto_Ref_(tt), mMax(max){}
	};
	
	template<typename TT>
	class DeleteArrayOp_
	{
	public:
		inline explicit DeleteArrayOp_(TT *t) {delete [] t;}
	};

	template<typename T, typename OP, typename REF>
	class Array_ : public Auto_<T, OP, REF>
	{
	public:
		inline Array_() : Auto_(new T[4]){}
		inline Array_(int size) : Auto_(new T[size]) {mRef->mMax = size;}
		inline Array_(T *t, int size) : Auto_(t) {mRef->mMax = size;}
		inline Array_(const Array_ &copy) : Auto_(copy){}
		inline Array_ &operator=(const Array_ &copy){return (Array_ &)Auto_::operator =(copy);}
		
		/* array apis */
		inline void displace(int index){
			memmove(mRef->ptr + index + 1, mRef->ptr + index, (mRef->mMax - index - 1) * sizeof(T));
			memset(mRef->ptr + index, 0, sizeof(T));
		}
		inline void expand(int max){
			max = mRef->mMax + (max < mRef->mMax ? mRef->mMax : max);
			T *new_ptr = new T[max];
			memcpy(new_ptr, mRef->ptr, mRef->mMax * sizeof(T));
			memset(mRef->ptr, 0, mRef->mMax * sizeof(T));
			delete [] mRef->ptr;
			mRef->ptr = new_ptr;
			mRef->mMax = max;
		}
		inline int size(){return mRef->mMax;}
	};

#define more(CLS)			Auto_<CLS, DeleteArrayOp_<CLS>, Auto_Ref_<CLS>>
#define arr(CLS)			Array_<CLS, DeleteArrayOp_<CLS>, Array_Ref_<CLS>>

	/* array of grab */
#define arr_grab(CLS)		arr(grab(CLS))

};

#endif

