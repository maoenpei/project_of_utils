

#include "AtlasBounding.h"

NS_DEF_NARAN{

	g2d::Sizei AtlasBounding::mergeImage(arr(g2d::Sizei *) inputs, arr(g2d::Recti *) outputs)
	{
		byte *data = new byte[mMaxSize.w * mMaxSize.h];
		return g2d::Sizei();
	}

	void AtlasBounding::setMaxSize(g2d::Sizei size)
	{
		mMaxSize = size;
	}

	void AtlasBounding::setBorderPadding(int padding)
	{
		mBorderPadding = padding;
	}

	void AtlasBounding::setShapePadding(int padding)
	{
		mShapePadding = padding;
	}

	void AtlasBounding::setAtlasPadding(int padding)
	{
		mAtlasPadding = padding;
	}

	void AtlasBounding::setFixed(bool isfixed)
	{
		mIsFixed = isfixed;
	}

	void AtlasBounding::setRotate(bool isrotate)
	{
		mIsRotate = isrotate;
	}

	void AtlasBounding::setPOT(bool isPOT)
	{
		mIsPOT = isPOT;
	}

	void AtlasBounding::setSquare(bool issquare)
	{
		mIsSquare = issquare;
	}

	grab(AtlasBounding) AtlasBounding::create()
	{
		grab(AtlasBounding) object(new AtlasBounding());
		return object;
	}

	AtlasBounding::AtlasBounding()
		: mMaxSize(4096, 4096)
		, mBorderPadding(1)
		, mShapePadding(1)
		, mAtlasPadding(0)
		, mIsFixed(false)
		, mIsRotate(true)
		, mIsPOT(false)
		, mIsSquare(false)
	{}

	AtlasBounding::~AtlasBounding()
	{}


}

