

#include "structure/KRSA.h"
#include "core/Utils.h"
#include "core/Array.h"

NS_DEF_NARAN{

	void RSA::generateOpposite()
	{
		// mKey * mOKey === 1 (mod mMod)
		Array<int> equations;
		int divide = mMod;
		int divisor = mKey;
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
		coef1 = coef1 % mMod;
		coef1 = (coef1 < 0 ? coef1 + mMod : coef1);
		mOKey = coef1;
	}

	u32 RSA::encode(u32 val)
	{
		return Utils::largeMultiMod(val, mKey, mMod);
	}

	u32 RSA::decode(u32 val)
	{
		return Utils::largeMultiMod(val, mOKey, mMod);
	}

	grab(RSA) RSA::create(int prime1, int prime2, int key)
	{
		return new RSA(prime1, prime2, key);
	}

	RSA::RSA(int prime1, int prime2, int key)
		: mKey(key)
		, mMod((prime1 - 1) * (prime2 - 1))
	{
		generateOpposite();
	}

	RSA::~RSA()
	{}

}

