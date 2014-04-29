
#include "core/AutoReference.h"
#include "core/CLibLoader.h"
#include "image/KImage.h"

using namespace NS_NARAN;

int main(int argc, char **argv)
{
	grab_arr(LibLoader) loaders(new grab(LibLoader)[3]);
	loaders[0] = LibLoader::create();
	loaders[0] = LibLoader::create();
	loaders[1] = LibLoader::create();
	loaders[2] = LibLoader::create();
	return 0;
}

