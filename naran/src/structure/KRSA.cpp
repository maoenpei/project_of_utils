

#include "structure/KRSA.h"
#include "core/Utils.h"
#include "core/Array.h"

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

	int RSA::getPrime1()
	{
		return mPrime1;
	}

	int RSA::getPrime2()
	{
		return mPrime2;
	}

	int RSA::getMod()
	{
		return mPrime1 * mPrime2;
	}

	int RSA::getKey()
	{
		return mKey;
	}

	int RSA::getOKey()
	{
		return mOKey;
	}

	grab(RSACoder) RSA::getEncoder()
	{
		return RSACoder::create(mKey, mPrime1 * mPrime2);
	}

	grab(RSACoder) RSA::getDecoder()
	{
		return RSACoder::create(mOKey, mPrime1 * mPrime2);
	}

	grab(RSA) RSA::create(int prime1, int prime2, int key)
	{
		return new RSA(prime1, prime2, key);
	}

	int RSA::genOKey(int omiga, int key)
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

	RSA::RSA(int prime1, int prime2, int key)
		: mPrime1(prime1)
		, mPrime2(prime2)
		, mKey(key)
		, mOKey(genOKey((prime1-1)*(prime2-1), key))
	{}

	RSA::~RSA()
	{}

}

