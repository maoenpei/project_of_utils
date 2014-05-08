#ifndef __NARAN_IMAGE_H__
#define __NARAN_IMAGE_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"
#include "structure/KGeometry.h"

NS_DEF_NARAN{

	typedef u32 pixel;
	typedef byte channel;
	typedef struct{channel b, g, r;} pxRGB;
	typedef struct{channel b, g, r, a;} pxRGBA;

	inline static pxRGB px2rgb(pixel c){
		pxRGB ret = {BIT_GETB(c, 2), BIT_GETB(c, 1), BIT_GETB(c, 0)};
		return ret;
	}
	inline static pixel rgb2px(pxRGB rgb){
		return BIT_PUSHB3(0xff, rgb.r, rgb.g, rgb.b);
	}
	inline static pxRGBA px2rgba(pixel c){
		return *((pxRGBA *)&c);
	}
	inline static pixel rgba2px(pxRGBA rgba){
		return *((pixel *)&rgba);
	}

	class CLS_EXPORT Image
	{
	public:
		pixel getPixel(int x, int y);
		void setPixel(int x, int y, pixel c);

		void reset();
		void resize(const g2d::Sizei &size, more(channel) data = nullof(channel));
		channel * getData();
		inline pxRGB *getRGB(){return (pxRGB *)getData();}
		inline pxRGBA *getRGBA(){return (pxRGBA *)getData();}
		
		int getWidth();
		int getHeight();
		const g2d::Sizei &getSize();

		void setAlpha(bool hasAlpha, const channel * alphas = NULL);
		bool getAlpha();
		int getChannels();

		grab(Image) getSubImage(g2d::Recti rect);
		void setSubImage(g2d::Pointi pt, grab(Image) img);
		grab(Image) rotateImage();

		static grab(Image) create();

	private:
		more(channel) mData;
		g2d::Sizei mSize;
		bool mHasAlpha;
		g2d::Recti adjustValidRect(const g2d::Recti &rect);
		
		CLS_HIDE(Image);
	};

};

#endif

