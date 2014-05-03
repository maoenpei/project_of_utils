

#include "filesystem/KPath.h"

#define PATH_SPLIT			'/'
#define PATH_SPLIT_WIN32	'\\'
#define CHECK_SPLIT(ch)		((ch) == PATH_SPLIT || (ch) == PATH_SPLIT_WIN32)

NS_DEF_NARAN{

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
		char *nameStart = mPathChars.get();
		char *nameEnd = mPathChars.get() + strlen(nameStart);
		char *pName = nameStart;
		while(*pName){
			char ch = *pName;
			if (CHECK_SPLIT(ch)){
				if (pName + 1 == nameEnd)
					nameEnd = pName;
				else
					nameStart = pName + 1;
			}
			pName ++;
		}
		int l = nameEnd - nameStart;
		more(char) pathName = new char[l+1];
		strncpy(pathName.get(), nameStart, l);
		pathName[l] = 0;
		return pathName;
	}

}

