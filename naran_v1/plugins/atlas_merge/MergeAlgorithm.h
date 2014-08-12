#ifndef __NARAN_MERGE_ALGORITHM_H__
#define __NARAN_MERGE_ALGORITHM_H__

#include "Naran.h"
#include "MergeStruct.h"

NS_DEF_NARAN{

	class MergeAlgorithm : public IMergeAlgorithm
	{
	public:
		virtual int merge(arr(ImageRectMerge *) objects, grab(Configuration) conf, g2d::Sizei &size);
	};

}

#endif

