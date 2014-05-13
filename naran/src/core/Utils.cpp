

#include "core/Utils.h"

NS_DEF_NARAN{

	more(char) Utils::copyStr(c_str s)
	{
		int l = strlen(s);
		char *s_copy = new char[l + 1];
		strcpy(s_copy, s);
		return s_copy;
	}

	u32 Utils::hashStr(c_str s)
	{
		u32 hash = 1;
		while(*s)
			hash = hash * 31 + *s++;
		return hash;
	}

	u32 Utils::largeMultiMod(u32 val, u32 coef, u32 mod)
	{
		int x = 1;
		int power = val % mod;
		for (int i = 0; i<=31; i++){
			if (coef & (1 << i))
				x = (x * power) % mod;
			power = (power * power) % mod;
		}
		return x;
	}

}

