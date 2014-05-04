

#include "image/KImage.h"

NS_DEF_NARAN{

	pixel Image::getPixel(int x, int y)
	{
		if (mData){
			int offset = y * mWidth + x;
			return (mHasAlpha ? 
				rgba2px(((pxRGBA *)mData)[offset]) : 
				rgb2px(((pxRGB *)mData)[offset]));
		}else
			return 0;
	}

	void Image::setPixel(int x, int y, pixel c)
	{
		if (mData){
			int offset = y * mWidth + x;
			(mHasAlpha ?
				(void)(((pxRGBA *)mData)[offset] = px2rgba(c)) : 
				(void)(((pxRGB *)mData)[offset] = px2rgb(c)));
		}
	}

	void Image::reset()
	{
		mData = nullof(channel);
	}

	void Image::resize(int width, int height, more(channel) data)
	{
		if (width <= 0 || height <= 0)
			return;
		mWidth = width;
		mHeight = height;
		if (data){
			mData = data;
		}else{
			int siz = width * height * (mHasAlpha ? 4 : 3);
			mData = new channel[siz] ();
		}
	}

	channel *Image::getData()
	{
		return mData;
	}

	int Image::getWidth()
	{
		return mWidth;
	}

	int Image::getHeight()
	{
		return mHeight;
	}

	void Image::setAlpha(bool hasAlpha, const channel *alphas)
	{
		if (!mHasAlpha != !hasAlpha){
			if (mData && mWidth > 0 && mHeight > 0){
				int px_count = mWidth * mHeight;
				int old_single = (mHasAlpha ? 4 : 3);
				channel *old_pdata = mData;
				int new_single = (hasAlpha ? 4 : 3);
				channel *new_data = new channel[px_count * new_single] ();
				channel *new_pdata = new_data;
				for (int i = 0; i<px_count; i++){
					*((pxRGB *)new_pdata) = *((pxRGB *)old_pdata);
					new_pdata += new_single;
					old_pdata += old_single;
				}
				mData = new_data;
			}
			mHasAlpha = hasAlpha;
		}
		if (mData && mHasAlpha && alphas){
			int px_count = mWidth * mHeight;
			pxRGBA *px_data = (pxRGBA *)mData;
			for (int i = 0; i<px_count; i++){
				px_data[i].a = alphas[i];
			}
		}
	}

	bool Image::getAlpha()
	{
		return mHasAlpha;
	}

	grab(Image) Image::create()
	{
		return grab(Image)(new Image());
	}

	Image::Image()
		: mWidth(0)
		, mHeight(0)
		, mHasAlpha(true)
	{}

	Image::~Image()
	{}

};
