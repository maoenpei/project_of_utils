

#include "KFile.h"

NS_DEF_NARAN{

	const Path &File::getPath()
	{
		return mPath;
	}

	int File::getSize()
	{
		if (mFilePtr){
			long cur = ftell(mFilePtr);
			fseek(mFilePtr, 0, SEEK_END);
			long last = ftell(mFilePtr);
			fseek(mFilePtr, cur, SEEK_SET);
			return last;
		}
		return 0;
	}

	int File::readBuffer(arr(byte) bs)
	{
		if (bs.count() > 0 && mFilePtr){
			return (int)fread(bs.get(), 1, bs.count(), mFilePtr);
		}
		return 0;
	}

	void File::writeBuffer(arr(byte) buf)
	{
		if (mFilePtr){
			fwrite(buf.get(), 1, buf.count(), mFilePtr);
		}
	}

	void File::open(const Path &path, u32 flags)
	{
		assert(mFilePtr == NULL);
		const char *ops = NULL;
		switch(flags)
		{
		case File_Read:
			ops = "rb";
			break;
		case File_Write:
			ops = "wb";
			break;
		case File_ReadWrite:
			ops = "rwb";
			break;
		case File_ReadCreate:
			ops = "rb+";
			break;
		case File_WriteCreate:
			ops = "wb+";
			break;
		}
		assert(ops && mFilePtr == NULL);
		mFilePtr = fopen(path.getChars().get(), ops);
		if (mFilePtr){
			mPath = path;
		}
	}

	void File::close()
	{
		if (mFilePtr){
			fclose(mFilePtr);
			mFilePtr = NULL;
			mPath.reset();
		}
	}

	grab(File) File::create()
	{
		grab(File) object(new File());
		return object;
	}

	grab(File) File::create(const Path &path)
	{
		grab(File) object(new File());
		object->open(path, File_Read);
		return object;
	}

	File::File()
	{}

	File::~File()
	{
		close();
	}

	
}

