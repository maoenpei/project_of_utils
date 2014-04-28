#ifndef __NARAN_AUTO_REFERENCE_H__
#define __NARAN_AUTO_REFERENCE_H__

#include <new>

namespace Naran {

#define AUTO_CLS_NAME			Auto_
#define AUTO_REF_CLS_NAME		Auto_Ref

	template<typename T>
	class AUTO_CLS_NAME
	{
		template<typename TT>
		struct AUTO_REF_CLS_NAME
		{
			AUTO_REF_CLS_NAME(TT *tt) : ptr(tt), ref(1){}
			TT *ptr;
			unsigned int ref;
		};
		AUTO_REF_CLS_NAME<T> *mRef;
	public:
		inline explicit AUTO_CLS_NAME(T *t) : mRef(new AUTO_REF_CLS_NAME<T>(t)){}
		inline AUTO_CLS_NAME(const AUTO_CLS_NAME &copy) : mRef(copy.mRef) {mRef->ref++;}
		inline ~AUTO_CLS_NAME(){
			if (--mRef->ref == 0 && mRef->ptr){mRef->ptr->destroy();}
		}

		inline AUTO_CLS_NAME &operator =(const AUTO_CLS_NAME &another){
			another.mRef->ref++; mRef->ref--; mRef = another.mRef;
			return *this;
		}
		inline T *operator ->(){return mRef->ptr;}

		inline T *get(){return mRef->ptr;}

	// forbid some methods
	private:
		void *operator new(size_t siz){return NULL;}
	};

#define AUTO_PROTOCOL()			public:virtual void destroy(){delete this;}
#define Auto(CLS)				AUTO_CLS_NAME<CLS>

};

#endif

