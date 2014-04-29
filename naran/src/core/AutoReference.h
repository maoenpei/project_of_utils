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
		inline Auto_() : mRef(0){}
		inline explicit Auto_(T *t) : mRef(new Auto_Ref_<T>(t)){}
		inline Auto_(const Auto_ &copy) : mRef(copy.mRef) {if (mRef) mRef->ref++;}
		inline Auto_ &operator =(const Auto_ &another){
			if (another.mRef) another.mRef->ref++;
			AUTO_EXEC_OP();
			mRef = another.mRef;
			return *this;
		}
		inline Auto_ &operator =(T *t){return operator =(Auto_(t));}
		inline ~Auto_(){AUTO_EXEC_OP();}
		inline T *operator ->(){return mRef->ptr;}
		inline T *get(){return mRef->ptr;}
		inline operator bool(){return mRef->ptr != 0;}
		inline T &operator[](int index){return mRef->ptr[index];}
	};

#undef AUTO_EXEC_OP

	/* auto drop object */
	template<typename T>
	class DestroyOp_
	{
	public:
		inline explicit DestroyOp_(T *t) {t->destroy();}
	};
#define AUTO_PROTOCOL()			public:virtual void destroy(){delete this;}
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

	/* array of drop */
#define grab_arr(CLS)		arr(grab(CLS))

};

#endif

