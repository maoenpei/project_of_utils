#ifndef __NARAN_MERGE_STAGE_H__
#define __NARAN_MERGE_STAGE_H__

#include "Naran.h"
#include "MergeStruct.h"
#include "ImageRectManager.h"

NS_DEF_NARAN{

	struct ImageInfo
	{
		g2d::Recti region;
		g2d::Recti atlas_region;
	};

	enum {
		ImageSort_None = 0,
		ImageSort_Width,
		ImageSort_Height,
		ImageSort_Area,
		ImageSort_Length,
	};

	class MergeStage
	{
	public:
		bool merge();

		bool makeImage(const char *path);

		void addImage(const char *path);
		void removeImage(const char *path);
		bool getImageData(const char *path, ImageInfo &info);

		void setTrim(bool isTrim);
		void setBorderPadding(int padding);
		void setInnerPadding(int padding);
		void setPOT(bool isPOT);
		void setSquare(bool isSquare);
		void setAllowRotation(bool isAllowRotation);
		void setSort(u32 sort);
		void setMaxSize(const g2d::Sizei &size);
		void setMaxWidth(int width);
		void setMaxHeight(int height);

		static grab(MergeStage) create();

	private:
		grab(Configuration) mConf;
		grab(ImageRectManager) mManager;
		StringMap(grab(ImageEntry)) mEntrys;
		bool mImageDirty;

		CLS_HIDE(MergeStage);
	};

}

#endif

