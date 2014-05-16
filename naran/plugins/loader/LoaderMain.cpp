
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
	grab(Directory) directory = Directory::create(
		FileSystem::shared()->getPluginDirectory());
	arr(DirProps) children = directory->enumChildren();
	for (int i = children.size()-1; i>=0; i--){
		if (! children[i].isDir){
			more(char) name = children[i].path->getRawName();
			Runtime::shared()->setCurrentName(name.get());
			LibLoader::create(children[i].path->getChars().get());
		}
	}
	
	Runtime::shared()->startup();

	const char *handlerName = 0;
	if (argc >= 3){
		if (0 == stricmp(argv[1], "-S")){
			handlerName = argv[2];
			argc -= 2;
			argv += 2;
		}
	}
	
	stable(IInputHandler) handler = Runtime::shared()->getInputHandler(handlerName);
	if (handler && argc > 0){
		arr(char *) args(new char *[argc-1], argc-1);
		for (int i = 1; i<argc; i++){
			args[i-1] = argv[i];
		}
		handler->run(args);
	}
	return 0;
}

