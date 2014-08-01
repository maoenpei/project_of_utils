#ifndef __NARAN_PATH_H__
#define __NARAN_PATH_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"

NS_DEF_NARAN{

	class CLS_EXPORT Path
	{
	public:
		more(char) getChars() const;
		void setChars(c_str path);
		
		more(char) getName() const;
		void setName(c_str name);
		more(char) getRawName() const;
		void setRawName(c_str rawName);
		more(char) getExtName() const;
		void setExtName(c_str extName);
		more(char) getDirectory() const;
		bool isRoot() const;
		void makeDirStyle();
		void reset();

		Path appendName(c_str subName) const;
		Path removeName() const;

		bool isFolder();

		Path();
		Path(c_str path);
		Path(more(char) path);
		
	private:
		more(char) mPathChars;
	};

}

#endif

