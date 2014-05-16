

#include "AtlasBounding.h"

#define NEXT_POT(x)			\
	do {\
		x--;\
		x |= (x >> 1);\
		x |= (x >> 2);\
		x |= (x >> 4);\
		x |= (x >> 8);\
		x |= (x >> 16);\
		x++;\
	} while(0)

NS_DEF_NARAN{

	struct StackNode{
		grab(g2d::Pointi) removed;
		int index;
		int inputed;
		int pointCount;
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
		
		point = points[i];
		points.remove(i)
		input(testing, point, rotate(sizes[n]), points)
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
		// outputs
		Array<grab(g2d::Recti)> rects;
		Array<grab(g2d::Pointi)> points;
		int maxCount = 0;
		// init
		points.append(new g2d::Pointi(mBorderPadding, mBorderPadding));
		more(StackNode) stacks = new StackNode[inputs.size()] ();
		int sIndex = 0;
		int pointIndex;
		stacks[sIndex].index = 0;
		stacks[sIndex].inputed = 1;
		stacks[sIndex].pointCount = 0;
		// call stack loop
		while(sIndex >= 0){
			bool removeNew = true;
			pointIndex = stacks[sIndex].index;
			if (pointIndex >= 0){
				// enter loop
				grab(g2d::Pointi) point = points[pointIndex];
				stacks[sIndex].removed = point;
				points.remove(pointIndex);
				g2d::Sizei tSize = *(inputs[sIndex]);
				if (! stacks[sIndex].inputed){
					tSize = g2d::Sizei(inputs[sIndex]->h, inputs[sIndex]->w);
				}
				if (inputRect(testing, point, tSize, rects, points)){
					// enter stack
					++sIndex;
					if (sIndex == inputs.size()){
						--sIndex;
						genMaxRects(rects, outputs, maxCount, size);
					}else{
						stacks[sIndex].index = points.length() - 1;
						stacks[sIndex].inputed = (mIsRotate ? 0 : 1);
						stacks[sIndex].pointCount = points.length() - 1;
						continue;
					}
				}else{
					genMaxRects(rects, outputs, maxCount, size);
					removeNew = false;
				}
			}else{
				// leave stack
				--sIndex;
			}
			if (sIndex >= 0){
				while(points.length() > stacks[sIndex].pointCount){
					points.remove(-1);
				}
				points.append(stacks[sIndex].removed, stacks[sIndex].index);
				stacks[sIndex].removed = nullof(g2d::Pointi);
				if (removeNew){
					testing->revert();
					rects.remove(-1);
				}
				if (stacks[sIndex].inputed){
					stacks[sIndex].index --;
					stacks[sIndex].inputed = (mIsRotate ? 0 : 1);
				}else{
					stacks[sIndex].inputed = 1;
				}
			}
		}
		return false;
	}

	bool AtlasBounding::inputRect(grab(AtlasTesting) testing, grab(g2d::Pointi) pt, const g2d::Sizei &size, Array<grab(g2d::Recti)> &rects, Array<grab(g2d::Pointi)> &points)
	{
		g2d::Recti putRect(pt->x, pt->y, size.w + mBorderPadding, size.h + mBorderPadding);
		if (! testing->testRect(putRect)){
			return false;
		}
		g2d::Recti itemRect(pt->x, pt->y, size.w + mShapePadding, size.h + mShapePadding);
		testing->cutRect(itemRect);
		if (! testing->testRect(itemRect)){
			return false;
		}
		testing->fillRect(itemRect);
		rects.append(new g2d::Recti(*pt, size));
		more(grab(g2d::Pointi)) just_points = new grab(g2d::Pointi)[3];
		just_points[0] = new g2d::Pointi(itemRect.pt.x + itemRect.size.w, itemRect.pt.y + itemRect.size.h);
		just_points[1] = new g2d::Pointi(itemRect.pt.x, itemRect.pt.y + itemRect.size.h);
		just_points[2] = new g2d::Pointi(itemRect.pt.x + itemRect.size.w, itemRect.pt.y);
		for (int i = 0; i<3; i++){
			grab(g2d::Pointi) pt = just_points[i];
			u32 flags = testing->testCross(*pt);
			if ((flags & AtlasTestingCross_Left || pt->x == mBorderPadding)
				&& (flags & AtlasTestingCross_Down || pt->y == mBorderPadding)){
				points.append(pt);
			}
		}
		return true;
	}

	void AtlasBounding::genMaxRects(Array<grab(g2d::Recti)> &rects, arr(g2d::Recti *) outputs, int &max, g2d::Sizei &size)
	{
		if (rects.length() < max){
			return;
		}
		if (rects.length() == max && mIsFixed){
			return;
		}
		if (mIsFixed){
			size = mMaxSize;
		}else{
			int r = 0, u = 0;
			for (int i = 0; i<rects.length(); i++){
				r = (rects[i]->pt.x + rects[i]->size.w > r ? rects[i]->pt.x + rects[i]->size.w : r);
				u = (rects[i]->pt.y + rects[i]->size.h > u ? rects[i]->pt.y + rects[i]->size.h : u);
			}
			r += mBorderPadding;
			u += mBorderPadding;
			if (mIsPOT){
				NEXT_POT(r);
				NEXT_POT(u);
			}
			if (mIsSquare){
				r = u = (r > u ? r : u);
			}
			if (size.w * size.h != 0 && size.w * size.h < r * u){
				return;
			}
			size = g2d::Sizei(r, u);
		}
		max = rects.length();
		for (int i = 0; i<max; i++){
			*(outputs[i]) = *(rects[i]);
		}
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
		, mIsFixed(false)
		, mIsRotate(true)
		, mIsPOT(false)
		, mIsSquare(false)
	{}

	AtlasBounding::~AtlasBounding()
	{}


}

