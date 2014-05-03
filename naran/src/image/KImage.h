#ifndef __NARAN_IMAGE_H__
#define __NARAN_IMAGE_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"

NS_DEF_NARAN{

	typedef u32 pixel;
	typedef byte channel;
	typedef struct{channel b, g, r;} pxRGB;
	typedef struct{channel b, g, r, a;} pxRGBA;

	static pxRGB px2rgb(pixel c){
		pxRGB ret = {BIT_GETB(c, 2), BIT_GETB(c, 1), BIT_GETB(c, 0)};
		return ret;
	}
	static pixel rgb2px(pxRGB rgb){
		return BIT_PUSHB3(0xff, rgb.r, rgb.g, rgb.b);
	}
	static pxRGBA px2rgba(pixel c){
		return *((pxRGBA *)&c);
	}
	static pixel rgba2px(pxRGBA rgba){
		return *((pixel *)&rgba);
	}

	class CLS_EXPORT Image
	{
	public:
		pixel getPixel(int x, int y);
		void setPixel(int x, int y, pixel c);

		void reset();
		void resize(int width, int height, more(channel) data = more(channel)());
		channel * getData();
		inline pxRGB *getRGB(){return (pxRGB *)getData();}
		inline pxRGBA *getRGBA(){return (pxRGBA *)getData();}
		
		int getWidth();
		int getHeight();

		void setAlpha(bool hasAlpha, const channel * alphas = NULL);
		bool getAlpha();

		static grab(Image) create();

	private:
		more(channel) mData;
		int mWidth;
		int mHeight;
		bool mHasAlpha;
		
		CLS_HIDE_ALL(Image);
	};

};

#endif

