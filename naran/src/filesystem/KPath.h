#ifndef __NARAN_PATH_H__
#define __NARAN_PATH_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"

NS_DEF_NARAN{

	class CLS_EXPORT Path
	{
	public:
		more(char) getChars();
		void setChars(c_str path);
		
		more(char) getName();
		void setName(c_str name);
		more(char) getDirectory();
		bool isRoot();
		void makeDirStyle();
		void reset();

		grab(Path) appendName(c_str subName);
		grab(Path) removeName();

		static grab(Path) create();
		static grab(Path) create(c_str path);
		
	private:
		more(char) mPathChars;

		CLS_HIDE(Path);
	};

}

#endif

