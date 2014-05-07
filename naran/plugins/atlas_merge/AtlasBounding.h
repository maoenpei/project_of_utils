#ifndef __NARAN_ATLAS_BOUNDING_H__
#define __NARAN_ATLAS_BOUNDING_H__

#include "Naran.h"

NS_DEF_NARAN{

	class CLS_EXPORT AtlasBounding
	{
	public:
		
		g2d::Sizei mergeImage(arr(g2d::Sizei *) inputs, arr(g2d::Recti *) outputs);
		
		void setMaxSize(g2d::Sizei size);
		void setBorderPadding(int padding);
		void setShapePadding(int padding);
		void setAtlasPadding(int padding);
		void setFixed(bool isfixed);
		void setRotate(bool isrotate);
		void setPOT(bool isPOT);
		void setSquare(bool issquare);

		static grab(AtlasBounding) create();
		
	private:
		g2d::Sizei	mMaxSize;			// size limit
		int			mBorderPadding;		// padding inside border
		int			mShapePadding;		// padding between shapes
		int			mAtlasPadding;		// padding inside every shape
		bool		mIsFixed;			// max size is fixed
		bool		mIsRotate;			// allow rotate
		bool		mIsPOT;				// force width/height power of two
		bool		mIsSquare;			// force width equals height

		CLS_HIDE(AtlasBounding);
	};

}

#endif

