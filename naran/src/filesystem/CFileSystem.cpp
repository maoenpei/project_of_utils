

#include "CFileSystem.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

NS_DEF_NARAN{

	grab(Path) FileSystem::getWorkingDirectory()
	{
#ifdef WIN32
		char pathBuf[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, pathBuf);
		return Path::create(pathBuf);
#else
		char pathBuf[MAX_SIZE];
		return Path::create(getcwd(pathBuf, MAX_SIZE));
#endif
	}

	void FileSystem::setWorkingDirectory(grab(Path) path)
	{
#ifdef WIN32
		SetCurrentDirectoryA(path->getChars().get());
#else
		chdir(path->getChars());
#endif
	}

	grab(Path) FileSystem::getWritableDirectory()
	{
		if (! mWritablePath){
			mWritablePath = Path::create("writable/");
		}
		return mWritablePath;
	}

	void FileSystem::setWritableDirectory(grab(Path) path)
	{
		mWritablePath = path;
	}

	grab(Path) FileSystem::getResourceDirectory()
	{
		if (! mResourcePath){
			mResourcePath = Path::create("resource/");
		}
		return mResourcePath;
	}

	void FileSystem::setResourceDirectory(grab(Path) path)
	{
		mResourcePath = path;
	}

	grab(Path) FileSystem::getPluginDirectory()
	{
		if (! mPluginPath){
			mPluginPath = Path::create("plugins/");
		}
		return mPluginPath;
	}

	void FileSystem::setPluginDirectory(grab(Path) path)
	{
		mPluginPath = path;
	}


}

