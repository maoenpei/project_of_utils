#ifndef __NARAN_FILE_H__
#define __NARAN_FILE_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"
#include "filesystem/KPath.h"

NS_DEF_NARAN{

	class CLS_EXPORT File
	{
	public:
		enum{
			File_Read = 0x1,
			File_Write = 0x2,
			File_Create = 0x4,
		};

	public:
		grab(Path) getPath();
		int getSize();

		arr(byte) readBuffer(int size = 0);
		void writeBuffer(arr(byte) buf);

		void open(grab(Path) path, u32 flags = File_Read | File_Write);
		void close();

		static grab(File) create();
		static grab(File) create(grab(Path) path);

	private:
		FILE *mFilePtr;
		grab(Path) mPath;

		CLS_HIDE(File);
	};

}

#endif

