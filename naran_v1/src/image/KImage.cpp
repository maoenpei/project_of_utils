

#include "KImage.h"

NS_DEF_NARAN{

	bool Image::isValid() const
	{
		return mData && mSize.w > 0 && mSize.h > 0 && mChannel > 0;
	}

	bool Image::inRegion(const g2d::Recti &rect) const
	{
		return mSize.contain(rect.pt) && mSize.contain(rect.getFar());
	}

	void Image::setData(more(byte) data, const g2d::Sizei &size, int channel)
	{
		mData = data;
		mSize = size;
		mChannel = channel;
	}

	more(byte) Image::getData() const
	{
		return mData;
	}

	const g2d::Sizei &Image::getSize() const
	{
		return mSize;
	}

	int Image::getChannel() const
	{
		return mChannel;
	}

	bool Image::getPixel(const g2d::Pointi &base, byte *px) const
	{
		if (!isValid() || !mSize.contain(base)){
			return false;
		}
		int offset = mChannel * (base.x + base.y * mSize.w);
		memcpy(px, mData.get() + offset, mChannel);
		return true;
	}

	bool Image::setPixel(const g2d::Pointi &base, const byte *px)
	{
		if (!isValid() || !mSize.contain(base)){
			return false;
		}
		int offset = mChannel * (base.x + base.y * mSize.w);
		memcpy(mData.get() + offset, px, mChannel);
		return true;
	}

	bool Image::compareImage(grab(Image) img) const
	{
		if (!isValid() || !img->isValid()){
			return false;
		}
		more(byte) data = img->getData();
		int channel = img->getChannel();
		const g2d::Sizei &size = img->getSize();
		if (mChannel != channel || mSize != size){
			return false;
		}
		return 0 == memcmp(mData.get(), data.get(), mChannel * (mSize.w * mSize.h));
	}

	bool Image::coverImage(grab(Image) img, const g2d::Pointi &base, const g2d::Recti &rect)
	{
		if (!isValid() || !img || !img->isValid()){
			return false;
		}
		g2d::Recti t_rect(base, rect.size);
		if (!inRegion(t_rect) || !img->inRegion(rect)){
			return false;
		}
		if (mChannel != img->getChannel()){
			return false;
		}
		const g2d::Sizei &size = img->getSize();
		more(byte) data = img->getData();
		byte *data_to = mData.get() + mChannel * (base.x + base.y * mSize.w);
		byte *data_from = data.get() + mChannel * (rect.pt.x + rect.pt.y * size.w);
		for (int i = 0; i<rect.size.h; i++){
			memcpy(data_to, data_from, mChannel * rect.size.w);
			data_to += mChannel * mSize.w;
			data_from += mChannel * size.w;
		}
		return true;
	}

	grab(Image) Image::subImage(const g2d::Recti &rect) const
	{
		if (!isValid() || !inRegion(rect)){
			return nullof(Image);
		}
		more(byte) data = new byte[mChannel * (rect.size.w * rect.size.h)];
		byte *data_to = data.get();
		byte *data_from = mData.get() + mChannel * (rect.pt.x + rect.pt.y * mSize.w);
		for (int i = 0; i<rect.size.h; i++){
			memcpy(data_to, data_from, mChannel * rect.size.w);
			data_to += mChannel * mSize.w;
			data_from += mChannel * rect.size.w;
		}
		grab(Image) img = new Image();
		img->setData(data, rect.size, mChannel);
		return img;
	}

	grab(Image) Image::rotateImage(int timesOfRightAngle) const
	{
		if (!isValid() || timesOfRightAngle < 0){
			return nullof(Image);
		}
		timesOfRightAngle = timesOfRightAngle % 4;
		more(byte) data = new byte[mChannel * (mSize.w * mSize.h)];
		g2d::Sizei size = ((timesOfRightAngle & 0x1) == 0 ? mSize : g2d::Sizei(mSize.h, mSize.w));
		if (timesOfRightAngle == 0) {
			memcpy(data.get(), mData.get(), mChannel * (mSize.w * mSize.h));
		}else{
			int x = 0, y = 0;
			int dx = 0, dy = 0;
			if (timesOfRightAngle == 1){
				x = 0; y = size.h - 1;
				dx = -size.w; dy = 1 + size.w * size.h;
			}else if (timesOfRightAngle == 2){
				x = size.w - 1; y = size.h - 1;
				dx = -1; dy = 0;
			}else if (timesOfRightAngle == 3){
				x = size.h - 1; y = 0;
				dx = size.w; dy = -1 - size.w * size.h;
			}
			byte *data_to = data.get() + mChannel * (x + y * size.w);
			byte *data_from = mData.get();
			for (int i = 0; i<mSize.h; i++){
				for (int j = 0; j<mSize.w; j++){
					memcpy(data_to, data_from, mChannel);
					data_from += mChannel;
					data_to += mChannel * dx;
				}
				data_to += mChannel * dy;
			}
		}
		grab(Image) img = new Image();
		img->setData(data, size, mChannel);
		return img;
	}

	grab(Image) Image::create(more(byte) data, const g2d::Sizei &size, int channel)
	{
		grab(Image) img = new Image();
		img->setData(data, size, channel);
		return img;
	}

	grab(Image) Image::create()
	{
		grab(Image) img = new Image();
		return img;
	}

	Image::Image()
	{}

	Image::~Image()
	{}

	
}

