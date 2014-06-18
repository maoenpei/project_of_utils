#ifndef __NARAN_RSA_H__
#define __NARAN_RSA_H__

#include "CommonDef.h"

NS_DEF_NARAN{

	class CLS_EXPORT RSACoder
	{
	public:
		inline int getKey(){return mKey;}
		inline int getMod(){return mMod;}

		u32 encode(u32 val);

		RSACoder(int key, int mod);

	private:
		int mKey;
		int mMod;
	};

	class CLS_EXPORT RSAProvider
	{
	public:

		inline int getPrime1(){return mPrime1;}
		inline int getPrime2(){return mPrime2;}
		inline int getMod(){return mPrime1 * mPrime2;}
		inline int getKey(){return mKey;}
		inline int getOKey(){return mOKey;}

		RSACoder * getEncoder();
		RSACoder * getDecoder();

		RSAProvider(int prime1, int prime2, int key);

	private:
		int mPrime1;
		int mPrime2;
		int mKey;
		int mOKey;

		int genOKey(int omiga, int key);
	};

}

#endif

