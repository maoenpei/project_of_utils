#ifndef __NARAN_ALLOCATOR_H__
#define __NARAN_ALLOCATOR_H__

#include "CommonDef.h"

NS_DEF_NARAN{

	class CLS_EXPORT Allocator
	{
	public:
		static void *allocShare(size_t siz);
		static void freeShare(void *ptr);
		static void *allocArrayShare(size_t siz);
		static void freeArrayShare(void *ptr);
	};

#define CLS_SHARE_MEMORY()			\
	void *operator new(size_t siz){return NS_NARAN::Allocator::allocShare(siz);}		\
	void operator delete(void *ptr){NS_NARAN::Allocator::freeShare(ptr);}			\
	void *operator new[](size_t siz){return NS_NARAN::Allocator::allocArrayShare(siz);}	\
	void operator delete[](void *ptr){NS_NARAN::Allocator::freeArrayShare(ptr);}

}

#endif

