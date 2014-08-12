#ifndef __NARAN_IMAGE_RECT_MANAGER_H__
#define __NARAN_IMAGE_RECT_MANAGER_H__

#include "Naran.h"
#include "MergeStruct.h"

NS_DEF_NARAN{

	struct ImageEntry
	{
		grab(Image) image;
		g2d::Recti region;
		ImageRectMergeStorage *rect;
		ImageEntry() : rect(NULL){}
	};

	class ImageRectManager
	{
	public:
		void reset();
		bool addImageRect(ImageEntry *entry, const char *path);
		
		arr(ImageRectMergeStorage *) getRects();

		static grab(ImageRectManager) create();
		
	private:
		Array<grab(ImageRectMergeStorage)> mRects;
		CLS_HIDE(ImageRectManager);
	};

}

#endif

