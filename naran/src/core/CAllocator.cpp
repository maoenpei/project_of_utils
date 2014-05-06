

#include "core/CAllocator.h"

NS_DEF_NARAN{

	void *Allocator::allocShare(size_t siz)
	{
		return ::operator new(siz);
	}

	void Allocator::freeShare(void *ptr)
	{
		::operator delete(ptr);
	}

	void *Allocator::allocArrayShare(size_t siz)
	{
		return ::operator new[](siz);
	}

	void Allocator::freeArrayShare(void *ptr)
	{
		::operator delete[](ptr);
	}


}

