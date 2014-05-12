#ifndef __NARAN_ATLAS_TESTING_H__
#define __NARAN_ATLAS_TESTING_H__

#include "Naran.h"

NS_DEF_NARAN{

	enum {
		AtlasTestingCross_Left = 0x1,
		AtlasTestingCross_Right = 0x2,
		AtlasTestingCross_Down = 0x4,
		AtlasTestingCross_Up = 0x8,
	};

	class CLS_EXPORT AtlasTesting
	{
	public:
		void fillRect(const g2d::Recti &rect);
		void revert();
		bool testRect(const g2d::Recti &rect);
		void cutRect(g2d::Recti &rect);
		u32 testCross(const g2d::Pointi &pt);

		int getCount();
		const g2d::Recti &getRect(int index);

		static grab(AtlasTesting) create(const g2d::Sizei &size);

	private:
		Array<g2d::Recti> mRects;
		g2d::Sizei mSize;
		more(byte) mData;
		more(byte) mZeroLine;

		AtlasTesting(const g2d::Sizei &size);
		
		CLS_HIDE(AtlasTesting);
	};

}

#endif

