

#include "filesystem/KDirectory.h"
#include "structure/KArray.h"

#include <direct.h>
#ifdef WIN32
#include <windows.h>
#include <io.h>
#define access(path, mode)		_access(path, mode)
#define mkdir(path, mode)		_mkdir(path)
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

NS_DEF_NARAN{

	grab(Path) Directory::getPath()
	{
		return mPath;
	}

	static bool mkdirs(grab(Path) path)
	{
		if (path->isRoot()){
			return true;
		}
		if (0 == access(path->getChars().get(), 0)){
			return true;
		}
		if (mkdirs(path->removeName())){
			return 0 == mkdir(path->getChars().get(), 0);
		}
		return false;
	}

	void Directory::makeDirs()
	{
		mkdirs(mPath);
	}

	class _IEnumDir
	{
	public:
		virtual bool checkStop(DirProps &prop) = 0;
	};

	static void enumFolder(grab(Path) path, _IEnumDir *enumer)
#ifdef WIN32
	{
		char szFind[MAX_PATH];
		WIN32_FIND_DATAA FindFileData;
		strcpy(szFind, path->getChars().get());
		strcat(szFind, "*");
		HANDLE hFind=::FindFirstFileA(szFind,&FindFileData);
		if (INVALID_HANDLE_VALUE != hFind){
			DirProps prop;
			do{
				if (0 == strcmp(FindFileData.cFileName, ".")
					|| 0 == strcmp(FindFileData.cFileName, ".."))
					continue;
				prop.path = path->appendName(FindFileData.cFileName);
				prop.isDir = (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
				prop.size = FindFileData.nFileSizeLow;
				if (! enumer->checkStop(prop)){
					break;
				}
			}while(FindNextFileA(hFind, &FindFileData));
			FindClose(hFind);
		}
	}
#else
	{
		char fullpath[MAX_SIZE];
		struct dirent *s_dir;
		struct stat file_stat;
		DirProps prop;
		DIR *dir = opendir(mPath->getChars());
		while ((s_dir = readdir(dir)) != NULL){
			if (0 == strcmp(s_dir->d_name, ".")
				|| 0 == strcmp(s_dir->d_name, ".."))
				continue;
			prop.path = mPath->appendName(s_dir->d_name);
			stat(prop.path->getChars(), &file_stat);
			prop.isDir = S_ISDIR(file_stat.st_mode) != 0;
			prop.size = file_stat.st_size;
			if (! enumer->checkStop(prop)){
				break;
			}
		}
		closedir(dir);
	}
#endif

	arr(DirProps) Directory::enumChildren()
	{
		class _EnumInst : public _IEnumDir{
		public:
			Array<DirProps> props;
			virtual bool checkStop(DirProps &prop)
			{
				props.append(prop);
				return true;
			}
		}enumer;
		enumFolder(mPath, &enumer);
		return enumer.props.toArr();
	}

	void Directory::enumTraverse(stable(IDirectoryTraverse) traverse, int recursive)
	{
		class _EnumInst : public _IEnumDir{
		public:
			stable(IDirectoryTraverse) mTraverse;
			int mRecursive;
			_EnumInst(stable(IDirectoryTraverse) traverse, int recursive)
				: mTraverse(traverse), mRecursive(recursive){}
			virtual bool checkStop(DirProps &prop)
			{
				/* less than 0 means recursive forever, more than 0 means recursive level */
				if (prop.isDir && (mRecursive < 0 || mRecursive > 0)){
					_EnumInst enumer(mTraverse, mRecursive-1);
					enumFolder(prop.path, &enumer);
					return true;
				}else{
					return mTraverse->dealWithDirectory(prop.path);
				}
			}
		} enumer(traverse, recursive);
		enumFolder(mPath, &enumer);
	}

	grab(Directory) Directory::create(grab(Path) path)
	{
		grab(Directory) object = new Directory(path);
		return object;
	}

	Directory::Directory(grab(Path) path)
		: mPath(path)
	{
		mPath->makeDirStyle();
	}

	Directory::~Directory()
	{}


}

