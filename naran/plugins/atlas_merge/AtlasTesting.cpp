

#include "AtlasTesting.h"

NS_DEF_NARAN{

	void AtlasTesting::fillRect(const g2d::Recti &rect)
	{
		byte *start = mData.get() + rect.pt.x + rect.pt.y * mSize.w;
		for (int i = 0; i<rect.size.h; i++){
			memset(start, 0xff, rect.size.w);
			start += mSize.w;
		}
		mRects.append(rect, 0);
	}

	void AtlasTesting::revert()
	{
		if (mRects.length() > 0){
			g2d::Recti rect = mRects[0];
			mRects.remove(0);
			byte *start = mData.get() + rect.pt.x + rect.pt.y * mSize.w;
			for (int i = 0; i<rect.size.h; i++){
				memset(start, 0, rect.size.w);
				start += mSize.w;
			}
		}
	}

	bool AtlasTesting::testRect(const g2d::Recti &rect)
	{
		if (rect.pt.x < 0 || rect.pt.y < 0
			|| rect.pt.x + rect.size.w > mSize.w
			|| rect.pt.y + rect.size.h > mSize.h){
			return false;
		}
		byte *start = mData.get() + rect.pt.x + rect.pt.y * mSize.w;
		for (int i = 0; i<rect.size.h; i++){
			if (0 != memcmp(mZeroLine.get(), start, rect.size.w)){
				return false;
			}
			start += mSize.w;
		}
		return true;
	}

	u32 AtlasTesting::testCross(const g2d::Pointi &pt)
	{
		u32 cross = 0;
		byte *center = mData.get() + pt.x + pt.y * mSize.w;
		if (pt.x > 0 && center[-1]){
			cross |= AtlasTestingCross_Left;
		}
		if (pt.x < mSize.w-1 && center[1]){
			cross |= AtlasTestingCross_Right;
		}
		if (pt.y > 0 && center[-mSize.w]){
			cross |= AtlasTestingCross_Down;
		}
		if (pt.y < mSize.h-1 && center[mSize.w]){
			cross |= AtlasTestingCross_Up;
		}
		return cross;
	}

	grab(AtlasTesting) AtlasTesting::create(const g2d::Sizei &size)
	{
		grab(AtlasTesting) object = new AtlasTesting(size);
		return object;
	}

	AtlasTesting::AtlasTesting(const g2d::Sizei &size)
		: mSize(size)
		, mData(new byte[size.w * size.h]())
		, mZeroLine(new byte[size.w]())
	{
	}

	AtlasTesting::~AtlasTesting()
	{}

}

