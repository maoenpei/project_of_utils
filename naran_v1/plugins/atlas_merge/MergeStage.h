#ifndef __NARAN_MERGE_STAGE_H__
#define __NARAN_MERGE_STAGE_H__

#include "Naran.h"

NS_DEF_NARAN{

	struct ImageRect
	{
		// info part
		g2d::Sizei size;

		// result part
		g2d::Pointi start_result;
		bool rotated_result;

		// other part
		bool marked;
	};

	class ISortAlgorithm
	{
	public:
		virtual arr(ImageRect *) sort(arr(ImageRect *) objects) = 0;
	};

	class IMergeAlgorithm
	{
	public:
		virtual g2d::Sizei merge(arr(ImageRect *) objects) = 0;
	};

	struct ImageEntry
	{
		more(char) path;
		grab(Image) image;
		g2d::Recti region;
		ImageRect *rect;
	};

	struct ImageInfo
	{
		g2d::Recti region;
		g2d::Recti atlas_region;
	};

	enum {
		ImageShape_Any = 0,
		ImageShape_POT,
	};

	class CLS_EXPORT MergeStage
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
		void setShape(u32 shape);
		void setSquare(bool isSquare);
		void setAllowRotation(bool isAllowRotation);

		grab(MergeStage) create();

	private:
		grab(Configuration) mConf;
		Array<grab(ImageRect)> mRects;
		StringMap(grab(ImageEntry)) mEntrys;

		CLS_HIDE(MergeStage);
	};

}

#endif

