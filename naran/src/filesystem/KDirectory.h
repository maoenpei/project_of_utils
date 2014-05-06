#ifndef __NARAN_DIRECTORY_H__
#define __NARAN_DIRECTORY_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"
#include "core/AutoCounter.h"
#include "filesystem/KPath.h"
#include "filesystem/KFile.h"

NS_DEF_NARAN{

	class Directory;
	class IDirectoryTraverse
	{
	public:
		virtual bool dealWithDirectory(grab(Path) path) = 0;
	};
	
	struct CLS_EXPORT DirProps{
		grab(Path) path;
		int size;
		bool isDir;
	};

	class CLS_EXPORT Directory
	{
	public:
		grab(Path) getPath();

		void makeDirs();

		arr(DirProps) enumChildren();
		void enumTraverse(stable(IDirectoryTraverse) traverse, int recursive = -1);

		static grab(Directory) create(grab(Path) path);

	private:
		grab(Path) mPath;

		CLS_HIDE(Directory);
		Directory(grab(Path) path);
	};

}

#endif

