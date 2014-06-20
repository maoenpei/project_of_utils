

#include "framework/Utils.h"

#if defined(WIN32)
#include <windows.h>
static inline int getTimeTick(int coef)
{
    LARGE_INTEGER liTime, liFreq;
    QueryPerformanceFrequency( &liFreq );
    QueryPerformanceCounter( &liTime );
	int sec = (int)(liTime.QuadPart / liFreq.QuadPart);
	int lTick = (int)(liTime.QuadPart % liFreq.QuadPart);
	int ssec = (int) ( lTick * (double)coef / liFreq.QuadPart);
    return sec * coef + ssec;
}
#else

#endif

NS_DEF_NARAN{

	more(char) Utils::copyStr(c_str s)
	{
		if (! s){
			return nullof(char);
		}
		int l = strlen(s);
		char *s_copy = new char[l + 1];
		strcpy(s_copy, s);
		return s_copy;
	}

	u32 Utils::hashStr(c_str s)
	{
		if (! s){
			return 0;
		}
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

	int Utils::tickMilli()
	{
		return getTimeTick(1000);
	}

}

