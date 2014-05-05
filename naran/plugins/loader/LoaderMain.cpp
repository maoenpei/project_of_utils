
#ifdef WIN32
#include <windows.h>
#endif

#include "Naran.h"

NS_USING_NARAN;

CLS_SHARE_MEMORY();

int main(int argc, char **argv)
{
#ifdef WIN32
	SetErrorMode(SEM_FAILCRITICALERRORS);
#endif
	Runtime::shared()->startup();
	grab(Directory) directory = Directory::create(
		FileSystem::shared()->getResourceDirectory());
	arr(Directory::DirProps) children = directory->enumChildren();
	for (int i = children.size()-1; i>=0; i--){
		LibLoader::create(children[i].path->getChars());
	}
	stable(IInputHandler) handler = Runtime::shared()->getInputHandler();
	if (handler && argc > 0){
		arr(char *) args(new char *[argc-1], argc-1);
		for (int i = 1; i<argc; i++){
			args[i-1] = argv[i];
		}
		handler->run(args);
	}
	return 0;
}

