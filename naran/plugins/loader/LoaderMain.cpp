
#ifdef WIN32
#include <windows.h>
#endif

#include "Naran.h"

NS_USING_NARAN;

CLS_SHARE_MEMORY();

//#define WAIT_DEBUG

int main(int argc, char **argv)
{
#ifdef WAIT_DEBUG
	char buf[1024];
	printf("type anything and press enter!");
	scanf("%s", buf);
#endif
#ifdef WIN32
	SetErrorMode(SEM_FAILCRITICALERRORS);
#endif
	grab(Directory) directory = Directory::create(
		FileSystem::shared()->getPluginDirectory());
	arr(DirProps) children = directory->enumChildren();
	for (int i = children.size()-1; i>=0; i--){
		if (! children[i].isDir){
			LibLoader::create(children[i].path->getChars().get());
		}
	}
	
	Runtime::shared()->startup();
	
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

