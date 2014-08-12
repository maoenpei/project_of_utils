#ifndef __NARAN_SORT_ALGORITHM_H__
#define __NARAN_SORT_ALGORITHM_H__

#include "Naran.h"
#include "MergeStruct.h"

NS_DEF_NARAN{

	class MergeSortAlgorithm : public ISortAlgorithm
	{
	public:
		virtual ~MergeSortAlgorithm();
		virtual arr(ImageRect *) sort(arr(ImageRect *) objects);

		// check rect1 > rect2
		virtual bool compareGreaterThan(ImageRect *rect1, ImageRect *rect2) = 0;
	};

	
}

#endif
