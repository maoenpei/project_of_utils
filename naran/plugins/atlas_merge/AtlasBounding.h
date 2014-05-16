#ifndef __NARAN_ATLAS_BOUNDING_H__
#define __NARAN_ATLAS_BOUNDING_H__

#include "Naran.h"
#include "AtlasTesting.h"

NS_DEF_NARAN{

	class CLS_EXPORT AtlasBounding
	{
	public:
		
		bool mergeImage(arr(g2d::Sizei *) inputs, arr(g2d::Recti *) outputs, g2d::Sizei &size);
		
		void setMaxSize(const g2d::Sizei &size);
		void setBorderPadding(int padding);
		void setShapePadding(int padding);
		void setFixed(bool isfixed);
		void setRotate(bool isrotate);
		void setPOT(bool isPOT);
		void setSquare(bool issquare);

		static grab(AtlasBounding) create();
		
	private:
		g2d::Sizei	mMaxSize;			// size limit
		int			mBorderPadding;		// padding inside border
		int			mShapePadding;		// padding between shapes
		bool		mIsFixed;			// max size is fixed
		bool		mIsRotate;			// allow rotate
		bool		mIsPOT;				// force width/height power of two
		bool		mIsSquare;			// force width equals height

		bool inputRect(grab(AtlasTesting) testing, grab(g2d::Pointi) pt, const g2d::Sizei &size, Array<grab(g2d::Recti)> &rects, Array<grab(g2d::Pointi)> &points);
		void genMaxRects(Array<grab(g2d::Recti)> &rects, arr(g2d::Recti *) outputs, int &max, g2d::Sizei &size);

		CLS_HIDE(AtlasBounding);
	};

}

#endif

