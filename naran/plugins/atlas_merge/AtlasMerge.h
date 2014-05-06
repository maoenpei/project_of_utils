#ifndef __NARAN_ATLAS_MERGE_H__
#define __NARAN_ATLAS_MERGE_H__

#include "Naran.h"

NS_DEF_NARAN{

	enum {
		AtlasOrder_Name,		// name character order
		AtlasOrder_Area,		// area large to small
		AtlasOrder_Width,		// width large to small
		AtlasOrder_Height,		// height large to small
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
		struct AtlasConfigData{
			g2d::Sizei	maxSize;			// size limit
			int			borderPadding;		// padding inside border
			int			shapePadding;		// padding between shapes
			int			atlasPadding;		// padding inside every shape
			bool		isFixed;			// max size is fixed
			bool		isRotate;			// allow rotate
			bool		isPOT;				// force width/height power of two
			bool		isSquare;			// force width equals height
			bool		isTrim;				// trim transparent
		};
		struct AtlasBlockData{
			g2d::Sizei		size;			// size
			g2d::Recti		atlascut;		// rect in atlas
		};
		struct AtlasImageData{
			grab(Image)				img;		// image ref
			more(char)				name;		// image name
			Array(more(char))		afters;		// follow names
			g2d::Recti				imgcut;		// rect in image
			grab(AtlasBlockData)	block;		// block ref
			AtlasImageData *		next;		// next data in setted order
		};
	public:
		// images
		void setImage(c_str name, grab(Image) img);
		void removeImage(c_str name);
		grab(Image) getImage(c_str name);

		// orders
		void resetOrder();
		void orderAfter(c_str name, c_str other);
		void setOrderType(u32 type, bool revert = false);
		void sort();

		// merge operation
		void merge();
		grab(AtlasInfo) getInfo(c_str name);
		arr(AtlasInfo) getAllInfo();
		grab(Image) genAtlas();

		// config
		void setMaxSize(g2d::Sizei size);
		void setBorderPadding(int padding);
		void setShapePadding(int padding);
		void setAtlasPadding(int padding);
		void setFixed(bool isfixed);
		void setRotate(bool isrotate);
		void setPOT(bool isPOT);
		void setSquare(bool issquare);

		// properties
	private:
		Array<grab(AtlasImageData)> mImages;
		AtlasImageData *mOrderedImages;
		Array<grab(AtlasBlockData)> mBlocks;
		AtlasConfigData mConfig;
		// dirty rules: 
		// order dirty/merge dirty;
		// order clean/merge dirty;
		// order clean/merge clean;
		void setOrderDirty();
		void setMergeDirty();
		
		CLS_HIDE(AtlasMerge);
	};

}

#endif

