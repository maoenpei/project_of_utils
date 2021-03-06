#ifndef __NARAN_FILE_SYSTEM_H__
#define __NARAN_FILE_SYSTEM_H__

#include "core/CommonDef.h"
#include "runtime/CRuntime.h"
#include "filesystem/KPath.h"

NS_DEF_NARAN{

	class CLS_EXPORT FileSystem
	{
	public:
		grab(Path) getWorkingDirectory();
		void setWorkingDirectory(grab(Path) path);

		grab(Path) getWritableDirectory();
		void setWritableDirectory(grab(Path) path);

		grab(Path) getResourceDirectory();
		void setResourceDirectory(grab(Path) path);

		grab(Path) getPluginDirectory();
		void setPluginDirectory(grab(Path) path);

	private:
		grab(Path) mWritablePath;
		grab(Path) mResourcePath;
		grab(Path) mPluginPath;

		CLS_SHARED(FileSystem);
	};

}

#endif
