

#include "filesystem/KPath.h"

#define PATH_SPLIT			'/'
#define PATHS_SPLIT			"/"
#define PATH_SPLIT_WIN32	'\\'
#define CHECK_SPLIT(ch)		((ch) == PATH_SPLIT || (ch) == PATH_SPLIT_WIN32)

NS_DEF_NARAN{

	static more(char) gEmptyPath;
	CLS_STATIC(Path, {
		char *n = new char[1];
		n[0] = 0;
		gEmptyPath = n;
	});

	inline static void getNames(char *path, char *&start, char *&end)
	{
		start = path;
		end = path + strlen(start);
		char *pName = start;
		while(*pName){
			char ch = *pName;
			if (CHECK_SPLIT(ch)){
				if (pName + 1 == end){
					end = pName;
				}else
					start = pName + 1;
			}
			pName ++;
		}
	}

	more(char) Path::getChars()
	{
		return mPathChars;
	}

	void Path::setChars(c_str path)
	{
		if (0 != strcmp(path, mPathChars.get())){
			mPathChars = new char[strlen(path) + 1];
			strcpy(mPathChars.get(), path);
		}
	}

	more(char) Path::getName()
	{
		char *nameStart, *nameEnd;
		getNames(mPathChars.get(), nameStart, nameEnd);
		int l = nameEnd - nameStart;
		char * pathName = new char[l+1];
		strncpy(pathName, nameStart, l);
		pathName[l] = 0;
		return pathName;
	}

	void Path::setName(c_str name)
	{
		char *path = mPathChars.get();
		char *nameStart, *nameEnd;
		getNames(path, nameStart, nameEnd);
		int namel = strlen(name), left = nameStart - path, right = strlen(nameEnd);
		int l = namel + left + right;
		char *newPath = new char[l + 1] ();
		strncpy(newPath, path, nameStart - path);
		strcat(newPath, name);
		if (namel > 0 && right > 0 && CHECK_SPLIT(name[namel-1]) && CHECK_SPLIT(nameEnd[0])){
			strcat(newPath, nameEnd + 1);
		}else
			strcat(newPath, nameEnd);
		mPathChars = newPath;
	}

	more(char) Path::getDirectory()
	{
		char *path = mPathChars.get();
		char *nameStart, *nameEnd;
		getNames(path, nameStart, nameEnd);
		int l = nameStart - path;
		char *dir = new char[l + 1];
		strncpy(dir, path, l);
		dir[l] = 0;
		return dir;
	}

	bool Path::isRoot()
	{
		char *path = mPathChars.get();
		char *nameStart, *nameEnd;
		getNames(path, nameStart, nameEnd);
#ifdef WIN32
		return (path == nameStart && nameEnd - nameStart > 2 && nameStart[1] == ':');
#else
		return (path == nameStart && nameEnd == nameStart);
#endif
	}

	void Path::makeDirStyle()
	{
		char *path = mPathChars.get();
		int l = strlen(path);
		if (l > 0 && CHECK_SPLIT(path[l-1])){
			return;
		}
		char *buf = new char[l+1+1];
		strcpy(buf, path);
		strcat(buf, PATHS_SPLIT);
		mPathChars = buf;
	}

	void Path::reset()
	{
		mPathChars = gEmptyPath;
	}

	grab(Path) Path::appendName(c_str subName)
	{
		char *path = mPathChars.get();
		int pathl = strlen(path);
		int l = pathl + 1 + strlen(subName);
		char *newPath = new char[l + 1] ();
		strcpy(newPath, path);
		if (pathl > 0 && !CHECK_SPLIT(path[pathl-1]))
			strcat(newPath, PATHS_SPLIT);
		strcat(newPath, subName);
		
		grab(Path) object(new Path());
		object->mPathChars = newPath;
		return object;
	}

	grab(Path) Path::removeName()
	{
		char *path = mPathChars.get();
		char *nameStart, *nameEnd;
		getNames(path, nameStart, nameEnd);
		int l = nameStart - path;
		char *dir = new char[l + 1];
		strncpy(dir, path, l);
		dir[l] = 0;
		
		grab(Path) object(new Path());
		object->mPathChars = dir;
		return object;
	}

	grab(Path) Path::create()
	{
		return new Path();
	}

	grab(Path) Path::create(c_str path)
	{
		grab(Path) object(new Path());
		object->setChars(path);
		return object;
	}

	Path::Path()
		: mPathChars(gEmptyPath)
	{}

	Path::~Path()
	{}

}

