

#include "image/KImage.h"

NS_DEF_NARAN{

#define CHANNELS(HASALPHA)			((HASALPHA) ? sizeof(pxRGBA) : sizeof(pxRGB))

	pixel Image::getPixel(int x, int y)
	{
		if (mData){
			int offset = y * mSize.w + x;
			return (mHasAlpha ? 
				rgba2px(((pxRGBA *)mData.get())[offset]) : 
				rgb2px(((pxRGB *)mData.get())[offset]));
		}else
			return 0;
	}

	void Image::setPixel(int x, int y, pixel c)
	{
		if (mData){
			int offset = y * mSize.w + x;
			(mHasAlpha ?
				(void)(((pxRGBA *)mData.get())[offset] = px2rgba(c)) : 
				(void)(((pxRGB *)mData.get())[offset] = px2rgb(c)));
		}
	}

	void Image::reset()
	{
		mData = nullof(channel);
	}

	void Image::resize(const g2d::Sizei &size, more(channel) data)
	{
		if (size.w <= 0 || size.h <= 0)
			return;
		mSize = size;
		if (data){
			mData = data;
		}else{
			int siz = mSize.w * mSize.h * CHANNELS(mHasAlpha);
			mData = new channel[siz] ();
		}
	}

	channel * Image::getData()
	{
		return mData.get();
	}

	int Image::getWidth()
	{
		return mSize.w;
	}

	int Image::getHeight()
	{
		return mSize.h;
	}

	const g2d::Sizei &Image::getSize()
	{
		return mSize;
	}

	void Image::setAlpha(bool hasAlpha, const channel *alphas)
	{
		if (!mHasAlpha != !hasAlpha){
			if (mData && mSize.w > 0 && mSize.h > 0){
				int px_count = mSize.w * mSize.h;
				int old_channels = CHANNELS(mHasAlpha);
				channel *old_pdata = mData.get();
				int new_channels = CHANNELS(hasAlpha);
				channel *new_data = new channel[px_count * new_channels];
				memset(new_data, 0xff, px_count * new_channels);
				channel *new_pdata = new_data;
				for (int i = 0; i<px_count; i++){
					*((pxRGB *)new_pdata) = *((pxRGB *)old_pdata);
					new_pdata += new_channels;
					old_pdata += old_channels;
				}
				mData = new_data;
			}
			mHasAlpha = hasAlpha;
		}
		if (mData && mHasAlpha && alphas){
			int px_count = mSize.w * mSize.h;
			pxRGBA *px_data = (pxRGBA *)mData.get();
			for (int i = 0; i<px_count; i++){
				px_data[i].a = alphas[i];
			}
		}
	}

	bool Image::getAlpha()
	{
		return mHasAlpha;
	}

	int Image::getChannels()
	{
		return CHANNELS(mHasAlpha);
	}

	grab(Image) Image::getSubImage(g2d::Recti rect)
	{
		rect = adjustValidRect(rect);

		if (rect.size.w <= 0 || rect.size.h <= 0){
			return nullof(Image);
		}
		channel *data = mData.get();
		int channels = CHANNELS(mHasAlpha);
		channel *subData = new channel[rect.size.w * rect.size.h * channels];
		channel *pSrc = data + channels * (rect.pt.x + rect.pt.y * mSize.w);
		channel *pDst = subData;
		for (int i = 0; i<rect.size.h; i++){
			memcpy(pDst, pSrc, channels * rect.size.w);
			pSrc += channels * mSize.w;
			pDst += channels * rect.size.w;
		}
		
		grab(Image) object = Image::create();
		object->setAlpha(mHasAlpha);
		object->resize(rect.size, subData);
		return object;
	}

	void Image::setSubImage(g2d::Pointi pt, grab(Image) img)
	{
		g2d::Recti rect(pt, img->getSize());
		g2d::Recti inRect = adjustValidRect(rect);
		if (inRect.size.w <= 0 || inRect.size.h <= 0){
			return;
		}
		g2d::Pointi imgPt(inRect.pt.x - rect.pt.x, inRect.pt.y - rect.pt.y);
		int channels = CHANNELS(mHasAlpha);
		channel *data = mData.get();
		const g2d::Sizei &imgSize = img->getSize();
		int imgChannels = img->getChannels();
		channel *imgData = img->getData();
		channel *pDst = data + channels * (inRect.pt.x + inRect.pt.y * mSize.w);
		channel *pSrc = imgData + imgChannels * (imgPt.x + imgPt.y * imgSize.w);
		if (channels == imgChannels){
			for (int i = 0; i<inRect.size.h; i++, pDst += channels * mSize.w, pSrc += imgChannels * imgSize.w){
				memcpy(pDst, pSrc, channels * inRect.size.w);
			}
		}else{
			for (int i = 0; i<inRect.size.h; i++, pSrc += channels * mSize.w, pDst += imgChannels * imgSize.w){
				memset(pDst, 0xff, channels * inRect.size.w);
				for (int i = 0; i<inRect.size.w; i++){
					*((pxRGB *)(pDst + channels * i)) = *((pxRGB *)pSrc + imgChannels * i);
				}
			}
		}
	}

	template<class T>
	static inline void rotateCopyData(channel *new_data, channel *data, int w, int h)
	{
		channel *pSrc = data;
		for (int i = 0; i<h; i++, pSrc += sizeof(T) * w){
			channel *pDst = new_data + sizeof(T) * (h - 1 - i);
			for (int j = 0; j<w; j++, pDst += sizeof(T) * h){
				*((T *)pDst) = ((T *)pSrc)[j];
			}
		}
	}

	grab(Image) Image::rotateImage()
	{
		channel *data = mData.get();
		int channels = CHANNELS(mHasAlpha);
		channel *new_data = new channel[channels * mSize.w * mSize.h];
		if (mHasAlpha){
			rotateCopyData<pxRGBA>(new_data, data, mSize.w, mSize.h);
		}else{
			rotateCopyData<pxRGB>(new_data, data, mSize.w, mSize.h);
		}

		grab(Image) object = Image::create();
		object->setAlpha(mHasAlpha);
		object->resize(g2d::Sizei(mSize.h, mSize.w), new_data);
		return object;
	}

	g2d::Recti Image::adjustValidRect(const g2d::Recti & rect)
	{
		// adjust size
		g2d::Pointi far = rect.getFar();
		int px = (rect.pt.x < 0 ? 0 : rect.pt.x);
		int py = (rect.pt.x < 0 ? 0 : rect.pt.x);
		far.x = (far.x > mSize.w ? mSize.w : far.x);
		far.y = (far.y > mSize.h ? mSize.h : far.y);
		return g2d::Recti(px, py, far.x - px, far.y - py);
	}

	grab(Image) Image::create()
	{
		return grab(Image)(new Image());
	}

	Image::Image()
		: mSize(0, 0)
		, mHasAlpha(true)
	{}

	Image::~Image()
	{}

};
