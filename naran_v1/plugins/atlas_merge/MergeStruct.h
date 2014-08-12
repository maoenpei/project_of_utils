#ifndef __NARAN_MERGE_STRUCT_H__
#define __NARAN_MERGE_STRUCT_H__

#include "Naran.h"

NS_DEF_NARAN{

	struct ImageRect
	{
		// info part
		g2d::Sizei size;
	};

	struct ImageRectMerge : public ImageRect
	{
		// merge result;
		g2d::Recti rect;
		bool markUsed;
	};

	struct ImageRectMergeStorage : public ImageRectMerge
	{
		grab(Image) cutImage;
	};

	class ISortAlgorithm
	{
	public:
		virtual arr(ImageRect *) sort(arr(ImageRect *) objects) = 0;
	};

	class IMergeAlgorithm
	{
	public:
		virtual int merge(arr(ImageRectMerge *) objects, grab(Configuration) conf, g2d::Sizei &size) = 0;
	};
	
}

#endif

