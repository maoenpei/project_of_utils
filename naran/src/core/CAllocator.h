#ifndef __NARAN_MEMORY_H__
#define __NARAN_MEMORY_H__

#include "core/CommonDef.h"

NS_DEF_NARAN{

	class CLS_EXPORT Allocator
	{
	public:
		static void *allocShare(size_t siz);
		static void freeShare(void *ptr);
	};

#define CLS_SHARE_MEMORY()			\
	void *operator new(size_t siz){return Allocator::allocShare(siz);}		\
	void operator delete(void *ptr){Allocator::freeShare(ptr);}			\
	void *operator new[](size_t siz){return Allocator::allocShare(siz);}	\
	void operator delete[](void *ptr){Allocator::freeShare(ptr);}

}

#endif

