
#include "core/AutoReference.h"
#include "core/CLibLoader.h"

using namespace Naran;

int main(int argc, char **argv)
{
	Auto(LibLoader) loader = LibLoader::create();
	return 0;
}

