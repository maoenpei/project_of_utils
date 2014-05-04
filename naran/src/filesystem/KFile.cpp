

#include "filesystem/KFile.h"

NS_DEF_NARAN{

	grab(Path) File::getPath()
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

	arr(byte) File::readBuffer(int size)
	{
		if (size <= 0)
			size = getSize();
		arr(byte) bs((size < 1 ? 1 : size));
		if (size > 0 && mFilePtr){
			size = (int)fread(bs.get(), 1, size, mFilePtr);
		}
		bs.resize(size);
		return bs;
	}

	void File::writeBuffer(arr(byte) buf)
	{
		if (mFilePtr){
			fwrite(buf.get(), 1, buf.size(), mFilePtr);
		}
	}

	void File::open(grab(Path) path, u32 flags)
	{
		assert(mFilePtr == NULL);
		const char *ops = NULL;
		switch(flags)
		{
		case File_Read:
			ops = "rb";
			break;
		case File_Read | File_Create:
			ops = "rb+";
			break;
		case File_Write:
			ops = "wb";
			break;
		case File_Write | File_Create:
			ops = "wb+";
			break;
		}
		assert(ops && mFilePtr == NULL);
		mFilePtr = fopen(path->getChars(), ops);
		if (mFilePtr){
			mPath = path;
		}
	}

	void File::close()
	{
		if (mFilePtr){
			fclose(mFilePtr);
			mFilePtr = NULL;
			mPath->reset();
		}
	}

	grab(File) File::create()
	{
		grab(File) object(new File());
		return object;
	}

	grab(File) File::create(grab(Path) path)
	{
		grab(File) object(new File());
		object->open(path, File_Read);
		return object;
	}

	File::File()
		: mPath(Path::create())
	{}

	File::~File()
	{
		close();
	}

	
}

