#ifndef __NARAN_FILE_H__
#define __NARAN_FILE_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"
#include "file/KPath.h"

NS_DEF_NARAN{

	enum{
		File_Read = 1,
		File_Write,
		File_ReadWrite,
		File_ReadCreate,
		File_WriteCreate,
	};

	class CLS_EXPORT File
	{
	public:
		const Path &getPath();
		int getSize();

		int readBuffer(arr(byte) bs);
		void writeBuffer(arr(byte) buf);

		void open(const Path &path, u32 flags = File_ReadWrite);
		void close();

		static grab(File) create();
		static grab(File) create(const Path &path);

	private:
		FILE *mFilePtr;
		Path mPath;

		CLS_HIDE(File);
	};

}

#endif

