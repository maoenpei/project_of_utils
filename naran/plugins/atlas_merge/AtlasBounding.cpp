

#include "AtlasBounding.h"

NS_DEF_NARAN{

	struct StackNode{
		grab(g2d::Pointi) removed;
		int index;
	};
	/*
function check(testing, points, sizes, n)
{
	int total = points.length();
	for (int i = total-1; i>=0; i--){
		point = points[i];
		points.remove(i)
		input(testing, point, sizes[n], points)
		check(testing, points, sizes, n+1);
		points.drop(total-1 ~)
		points.append(point);
	}
}
	*/

	bool AtlasBounding::mergeImage(arr(g2d::Sizei *) inputs, arr(g2d::Recti *) outputs, g2d::Sizei &size)
	{
		if (mMaxSize.w <= mBorderPadding * 2 || mMaxSize.h <= mBorderPadding * 2){
			return false;
		}
		grab(AtlasTesting) testing = AtlasTesting::create(mMaxSize);
		// fake call stack
		Array<grab(g2d::Pointi)> points;
		points.append(new g2d::Pointi(mBorderPadding, mBorderPadding));
		more(StackNode) stacks = new StackNode[inputs.size()] ();
		int sIndex = 0;
		stacks[sIndex].index = 0;
		bool isPop = false;
		while(sIndex >= 0){
			if (stacks[sIndex].index >= 0){
				// enter loop
				grab(g2d::Pointi) point = points[stacks[sIndex].index];
				stacks[sIndex].removed = point;
				points.remove(stacks[sIndex].index);
				if (inputRect(testing, point, *(inputs[sIndex]), points)){
					// enter stack
					++sIndex;
					stacks[sIndex].index = points.length() - 1;
					continue;
				}
				// pop to next loop
				isPop = true;
			}else{
				// leave stack
				--sIndex;
				// pop to upper next loop
				isPop = true;
			}
			if (isPop && sIndex >= 0){
				isPop = false;
				while(points.length() > stacks[sIndex].index){
					points.remove(stacks[sIndex].index - 1);
				}
				points.append(stacks[sIndex].removed);
				stacks[sIndex].removed = nullof(g2d::Pointi);
				testing->revert();
				stacks[sIndex].index --;
			}
		}
		return false;
	}

	bool AtlasBounding::inputRect(grab(AtlasTesting) testing, grab(g2d::Pointi) pt, const g2d::Sizei &size, Array<grab(g2d::Pointi)> &points)
	{
		g2d::Recti putRect(pt->x, pt->y, size.w + mBorderPadding, size.h + mBorderPadding);
		if (! testing->testRect(putRect)){
			return false;
		}
		g2d::Recti itemRect(*pt, size);
		testing->fillRect(itemRect);
		return true;
	}

	void AtlasBounding::setMaxSize(const g2d::Sizei &size)
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

