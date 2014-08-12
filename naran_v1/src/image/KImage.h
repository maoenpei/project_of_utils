#ifndef __NARAN_IMAGE_H__
#define __NARAN_IMAGE_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"
#include "structure/KGeometry.h"

NS_DEF_NARAN{

	class CLS_EXPORT Image
	{
	public:
		bool isValid() const;
		bool inRegion(const g2d::Recti &rect) const;
		void setData(more(byte) data, const g2d::Sizei &size, int channel);
		more(byte) getData() const;
		const g2d::Sizei &getSize() const;
		int getChannel() const;
		
		bool setPixel(const g2d::Pointi &base, const byte *px);
		bool getPixel(const g2d::Pointi &base, byte *px) const;
		
		bool compareImage(grab(Image) img) const;
		bool coverImage(grab(Image) img, const g2d::Pointi &base, const g2d::Recti &rect);
		grab(Image) subImage(const g2d::Recti &rect) const;
		grab(Image) rotateImage(int timesOfRightAngle) const;//counter clockwise rotation
		
		static grab(Image) create(more(byte) data, const g2d::Sizei &size, int channel);
		static grab(Image) create();
		
	private:
		more(byte) mData;
		g2d::Sizei mSize;
		int mChannel;
		
		CLS_HIDE(Image);
	};

}

#endif

