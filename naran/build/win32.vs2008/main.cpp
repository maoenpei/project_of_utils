
#include "Naran.h"
#ifdef WIN32
#include <windows.h>
#endif

NS_USING_NARAN;

class Interface
{
public:
	virtual void interf() = 0;
};

class A : public Interface
{
public:
	int c;
	A(){
	}
	~A(){
	}
	virtual void interf()
	{
	}
};

stable(Interface) getfunc()
{
	grab(A) object(new A);
	return stablize_grab(Interface, A, object);
}

typedef LongInt<2>	LLI;

int main(int argc, char **argv)
{
#if 0
	grab(RSA) rsa = RSA::create(43, 59, 13);
	grab(RSACoder) encoder = rsa->getEncoder();
	grab(RSACoder) decoder = rsa->getDecoder();
	for (int i = 11; i<500; i+=13){
		u32 k = encoder->encode(i);
		u32 z = decoder->encode(k);
		if (z != i){
			int cc = 0;
			k = encoder->encode(i);
			z = decoder->encode(k);
		}else{
			int uu = 0;
		}
	}
#endif
#if 0
	Runtime::shared()->startup();
	grab(Image) img = ImageFactory::shared()->loadFile("zzzz.png", ImageFormat_PNG);
	grab(Image) img2 = ImageFactory::shared()->loadFile("zzzz.jpg", ImageFormat_JPEG);
#endif
#if 0
	arr_grab_stable(Interface) cls_arr2 = (new grab_stable(Interface)[3]);
	grab_stable(Interface) cls = new stable(Interface)(getfunc());
	cls_arr2[0] = cls;
	cls_arr2[1] = cls;
	cls_arr2[2] = cls;
	(*cls_arr2[0])->interf();
#endif
#if 0
	stable_arr(Interface) cls_arr2;
	stable_arr(Interface) cls_arr;
	stable(Interface) cls = getfunc();
	cls_arr.append(cls);
	cls_arr2 = cls_arr;
	cls_arr2.remove(0);
#endif
#if 0
	grab_arr(LibLoader) loaders(new grab(LibLoader)[3]);
	loaders[0] = LibLoader::create();
	loaders[0] = LibLoader::create();
	loaders[1] = LibLoader::create();
	loaders[2] = LibLoader::create();
#endif
	return 0;
}

