

#include "structure/KRSA.h"
#include "core/Utils.h"
#include "structure/KArray.h"

NS_DEF_NARAN{

	int RSACoder::getKey()
	{
		return mKey;
	}

	int RSACoder::getMod()
	{
		return mMod;
	}

	u32 RSACoder::encode(u32 val)
	{
		return Utils::largeMultiMod(val, mKey, mMod);
	}

	grab(RSACoder) RSACoder::create(int key, int mod)
	{
		return new RSACoder(key, mod);
	}

	RSACoder::RSACoder(int key, int mod)
		: mKey(key)
		, mMod(mod)
	{}

	RSACoder::~RSACoder()
	{}

	int RSAProvider::getPrime1()
	{
		return mPrime1;
	}

	int RSAProvider::getPrime2()
	{
		return mPrime2;
	}

	int RSAProvider::getMod()
	{
		return mPrime1 * mPrime2;
	}

	int RSAProvider::getKey()
	{
		return mKey;
	}

	int RSAProvider::getOKey()
	{
		return mOKey;
	}

	grab(RSACoder) RSAProvider::getEncoder()
	{
		return RSACoder::create(mKey, mPrime1 * mPrime2);
	}

	grab(RSACoder) RSAProvider::getDecoder()
	{
		return RSACoder::create(mOKey, mPrime1 * mPrime2);
	}

	grab(RSAProvider) RSAProvider::create(int prime1, int prime2, int key)
	{
		return new RSAProvider(prime1, prime2, key);
	}

	int RSAProvider::genOKey(int omiga, int key)
	{
		// key * okey === 1 (mod omiga)
		Array<int> equations;
		int divide = omiga;
		int divisor = key;
		int quot;
		int remain;
		do {
			quot = divide / divisor;
			remain = divide % divisor;
			equations.append(quot);
			assert(remain != 0);
			divide = divisor;
			divisor = remain;
		} while(remain != 1);
		int coef1 = 1, coef2 = 0;
		for (int index = equations.length() - 1; index >= 0; index--){
			int new_coef1 = coef1 * (-equations[index]) + coef2;
			int new_coef2 = coef1;
			coef1 = new_coef1;
			coef2 = new_coef2;
		}
		coef1 = coef1 % omiga;
		coef1 = (coef1 < 0 ? coef1 + omiga : coef1);
		return coef1;
	}

	RSAProvider::RSAProvider(int prime1, int prime2, int key)
		: mPrime1(prime1)
		, mPrime2(prime2)
		, mKey(key)
		, mOKey(genOKey((prime1-1)*(prime2-1), key))
	{}

	RSAProvider::~RSAProvider()
	{}

}

