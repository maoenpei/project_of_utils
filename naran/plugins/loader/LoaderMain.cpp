

#include "Naran.h"

NS_USING_NARAN;

int main(int argc, char **agv)
{
	Runtime::shared()->startup();
	grab(Directory) directory = Directory::create(
		FileSystem::shared()->getResourceDirectory());
	arr(Directory::DirProps) children = directory->enumChildren();
	for (){
	}
	return 0;
}

