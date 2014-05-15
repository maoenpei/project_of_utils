#ifndef __NARAN_RSA_H__
#define __NARAN_RSA_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"

NS_DEF_NARAN{

	class CLS_EXPORT RSACoder
	{
	public:
		int getKey();
		int getMod();

		u32 encode(u32 val);

		static grab(RSACoder) create(int key, int mod);

	private:
		int mMod;
		int mKey;

	private:
		RSACoder(int key, int mod);
		CLS_HIDE(RSACoder);
	};

	class CLS_EXPORT RSA
	{
	public:

		int getPrime1();
		int getPrime2();
		int getMod();
		int getKey();
		int getOKey();

		grab(RSACoder) getEncoder();
		grab(RSACoder) getDecoder();

		static grab(RSA) create(int prime1, int prime2, int key);

	private:
		int mPrime1;
		int mPrime2;
		int mKey;
		int mOKey;

		int genOKey(int omiga, int key);
		
	private:
		RSA(int prime1, int prime2, int key);
		CLS_HIDE(RSA);
	};

}

#endif

