#ifndef __NARAN_ATLAS_MERGE_H__
#define __NARAN_ATLAS_MERGE_H__

#include "Naran.h"
#include "AtlasBounding.h"

NS_DEF_NARAN{

	enum {
		AtlasOrder_Name = 0,	// name character order
		AtlasOrder_Area,		// area large to small
		AtlasOrder_Width,		// width large to small
		AtlasOrder_Height,		// height large to small
		AtlasOrder_Max
	};

	struct CLS_EXPORT AtlasInfo
	{
		more(char)		name;		// atlas name
		g2d::Recti		imgcut;		// rect in image
		g2d::Pointi		offset;		// offset in atlas
		bool			rotated;	// rotated
	};
	
	class CLS_EXPORT AtlasMerge
	{
		struct AtlasBlockData{
			g2d::Sizei		size;			// size
			g2d::Recti		atlascut;		// rect in atlas
			int				marked;			// marked count
		};
		struct AtlasImageData{
			AtlasImageData(const String &_name, grab(Image) _img) : name(_name), img(_img), after(""){}
			// members
			String					name;		// image name
			grab(Image)				img;		// image ref
			g2d::Recti				imgcut;		// rect in image
			grab(AtlasBlockData)	block;		// block ref
			// members not used by self
			String					after;		// follow name
			AtlasImageData *		next;		// next data in setted order
		};
	public:
		// images
		void setImage(c_str name, grab(Image) img);
		void removeImage(c_str name);
		grab(Image) getImage(c_str name);
		void setTrim(bool istrim);
		void setInnerPadding(int padding);
		void clean();

		// orders
		void resetOrder();
		void orderAfter(c_str name, c_str other);
		void setOrderType(u32 type, bool revert = false);
		void sort();

		// merge operation
		void merge();
		grab(AtlasInfo) getInfo(c_str name);
		arr(AtlasInfo) getAllInfo();
		arr(more(char)) getMergeInfo(c_str name);
		grab(Image) genAtlas();

		// atlas config
		void setMaxSize(const g2d::Sizei &size);
		void setBorderPadding(int padding);
		void setShapePadding(int padding);
		void setFixed(bool isfixed);
		void setRotate(bool isrotate);
		void setPOT(bool isPOT);
		void setSquare(bool issquare);

		static grab(AtlasMerge) create();

		// properties
	private:
		Array<grab(AtlasImageData)> mImages;
		
		int mInnerPadding;
		bool mImageDirty;
		bool mIsTrim;
		
		AtlasImageData *mOrderedImages;
		bool mIsRevert;
		u32 mOrderType;

		grab(AtlasBounding) mBounding;
		bool mMergeDirty;
		g2d::Sizei mAtlasSize;
		arr(AtlasImageData *) mBlockBelongs;
		bool mMergeSuccess;

		// dirty rules: 
		// image dirty > order dirty > merge dirty
		void setImageDirty();
		void setOrderDirty();
		void setMergeDirty();
		
		// get index of a name
		int indexOfName(const String &name);
		// get valid rect of image
		void getValidRegion(grab(AtlasImageData) imgData);
		bool isRegionEqual(grab(AtlasImageData) imgData, grab(AtlasImageData) imgData2);
		void createBlock(grab(AtlasImageData) imgData);

		// sort algrithm
		arr(AtlasImageData *) mergeSort(arr(AtlasImageData *) datas);
		bool smallerImages(AtlasImageData *data, AtlasImageData *data2);
		void linkImages(arr(AtlasImageData *) datas);

		// info fill
		void atlasInfoFill(AtlasImageData *img, AtlasInfo *info);
		void atlasFill(grab(Image) img, AtlasImageData *data);
		
		CLS_HIDE(AtlasMerge);
	};

}

#endif

