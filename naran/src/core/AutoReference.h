#ifndef __NARAN_AUTO_REFERENCE_H__
#define __NARAN_AUTO_REFERENCE_H__

#include "core/CommonDef.h"

NS_DEF_NARAN{

#define AUTO_EXEC_OP()		\
	if (mRef && --mRef->ref == 0){\
		if (mRef->ptr) OP(mRef->ptr);\
		delete mRef;\
	}

	template<typename T, typename OP>
	class Auto_
	{
	protected:
		template<typename TT>
		struct Auto_Ref_
		{
			Auto_Ref_(TT *tt) : ptr(tt), ref(1){}
			TT *ptr;
			unsigned int ref;
		};
		Auto_Ref_<T> *mRef;
	public:
		/* ptr reference record */
		inline Auto_() : mRef(0){}
		inline /* explict */ Auto_(T *t) : mRef(new Auto_Ref_<T>(t)){}
		inline Auto_(const Auto_ &copy) : mRef(copy.mRef) {if (mRef) mRef->ref++;}
		inline Auto_ &operator =(const Auto_ &another){
			if (another.mRef) another.mRef->ref++;
			AUTO_EXEC_OP();
			mRef = another.mRef;
			return *this;
		}
		inline ~Auto_(){AUTO_EXEC_OP();}

		/* ptr usage */
		inline T *get(){return mRef->ptr;}
		inline T *operator ->(){return mRef->ptr;}
		inline T &operator *(){return *mRef->ptr;}
		inline T &operator[](int index){return mRef->ptr[index];}

		/* ptr special usage */
		inline void displace(int index, int total){
			memmove(mRef->ptr + index + 1, mRef->ptr + index, (total - index - 1) * sizeof(T));
			memset(mRef->ptr + index, 0, sizeof(T));
		}
		inline void swap(T *ptr, int total){
			memcpy(ptr, mRef->ptr, total * sizeof(T));
			memset(mRef->ptr, 0, total * sizeof(T));
		}
		
		/* ptr convertion */
		inline operator bool(){return mRef->ptr != 0;}
		template<class U>
		inline operator U * (){return (U *)mRef->ptr;}
	};

#undef AUTO_EXEC_OP

	/* auto delete object */
	template<typename T>
	class DestroyOp_
	{
	public:
		inline explicit DestroyOp_(T *t) {delete t;}
	};
//#define AUTO_PROTOCOL()			virtual void destroy(){delete this;}
#define HIDE_CLS_ALL(CLS)		protected:CLS();CLS(const CLS&copy);~CLS();friend class DestroyOp_<CLS>;
#define grab(CLS)				Auto_<CLS, DestroyOp_<CLS>>
#define g(CLS)					grab(CLS)

	/* auto delete array */
	template<typename T>
	class DeleteArrayOp_
	{
	public:
		inline explicit DeleteArrayOp_(T *t) {delete [] t;}
	};
#define arr(CLS)			Auto_<CLS, DeleteArrayOp_<CLS>>

	/* array of grab */
#define arr_grab(CLS)		arr(grab(CLS))

};

#endif

