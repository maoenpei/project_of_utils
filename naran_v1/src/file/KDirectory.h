#ifndef __NARAN_DIRECTORY_H__
#define __NARAN_DIRECTORY_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"
#include "autoref/AutoCounter.h"
#include "file/KPath.h"

NS_DEF_NARAN{

	class Directory;
	class IDirectoryTraverse
	{
	public:
		virtual bool dealWithDirectory(const Path &path) = 0;
	};
	
	struct CLS_EXPORT DirProps{
		Path path;
		int size;
		bool isDir;
	};

	class CLS_EXPORT Directory
	{
	public:
		const Path &getPath();

		void makeDirs();

		arr(DirProps) enumChildren();
		void enumTraverse(interf(IDirectoryTraverse) traverse, int recursive = -1);

		static grab(Directory) create(const Path &path);

	private:
		Path mPath;

		CLS_HIDE(Directory);
		Directory(const Path &path);
	};

}

#endif

