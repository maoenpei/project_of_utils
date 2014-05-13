#ifndef __NARAN_RSA_H__
#define __NARAN_RSA_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"

NS_DEF_NARAN{

	class CLS_EXPORT RSA
	{
	public:

		u32 encode(u32 val);
		u32 decode(u32 val);

		static grab(RSA) create(int prime1, int prime2, int key);

	private:
		int		mKey;		// private key
		int		mBig;		// prime_1 * prime_2
		int		mMod;		// (prime_1 - 1) * (prime_2 - 1)
		int		mOKey;		// inverse private key

		void generateOpposite();
		
	private:
		RSA(int prime1, int prime2, int key);
		CLS_HIDE(RSA);
	};

}

#endif

