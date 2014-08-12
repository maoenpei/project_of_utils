

#include "FillTesting.h"

NS_DEF_NARAN{

	const g2d::Sizei &FillTesting::getSize()
	{
		return mSize;
	}

	void FillTesting::fillRect(const g2d::Recti &rect)
	{
		byte *start = mData.get() + rect.pt.x + rect.pt.y * mSize.w;
		for (int i = 0; i<rect.size.h; i++){
			if (i == 0 || i == rect.size.h-1){
				memset(start, 0xff, rect.size.w);
			}else{
				start[0] = 0xff;
				start[rect.size.w-1] = 0;
			}
			start += mSize.w;
		}
		mRects.insert(rect);
	}

	void FillTesting::revert()
	{
		if (mRects.count() > 0){
			g2d::Recti rect = mRects[-1];
			mRects.remove(-1);
			byte *start = mData.get() + rect.pt.x + rect.pt.y * mSize.w;
			for (int i = 0; i<rect.size.h; i++){
				if (i == 0 || i == rect.size.h-1){
					memset(start, 0, rect.size.w);
				}else{
					start[0] = 0;
					start[rect.size.w-1] = 0;
				}
				start += mSize.w;
			}
		}
	}

	bool FillTesting::testRect(const g2d::Recti &rect)
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

	void FillTesting::cutRect(g2d::Recti &rect)
	{
		rect = g2d::Recti(0, 0, mSize.w, mSize.h).intersect(rect);
	}

	u32 FillTesting::testCross(const g2d::Pointi &pt)
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

	int FillTesting::getCount()
	{
		return mRects.count();
	}

	const g2d::Recti &FillTesting::getRect(int index)
	{
		return mRects[index];
	}

	grab(FillTesting) FillTesting::create(const g2d::Sizei &size)
	{
		grab(FillTesting) object = new FillTesting(size);
		return object;
	}

	FillTesting::FillTesting(const g2d::Sizei &size)
		: mSize(size)
		, mData(new byte[size.w * size.h]())
		, mZeroLine(new byte[size.w]())
	{
	}

	FillTesting::~FillTesting()
	{}

}

