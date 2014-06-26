
#include "structure/KLongLong.h"
#include "framework/Utils.h"
#include "file/KPath.h"
#include "structure/KHashmap.h"
#include "autoref/AutoCounter.h"
#include <pthread.h>
#include "algrithm/KRSA.h"

using namespace Naran;

int g_k = 0;

class A
{
};

class B
{
};

int main(int argc, char **argv)
{
	/*
	int key = 41213;
	RSAProvider rsa(211, 223, key);
	int n = rsa.getOKey();
	int m = rsa.getMod();
	for (int z = 0; z<=5000; z++){
		u32 code = Utils::largeMultiMod(z, n, m);
		u32 s = Utils::largeMultiMod(code, key, m);
		assert(s == z);
	}
	*/
	return 0;
}


