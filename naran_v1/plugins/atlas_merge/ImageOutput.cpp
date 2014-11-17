

#include "ImageOutput.h"


NS_DEF_NARAN{

	grab(ImageOutput) ImageOutput::shared()
	{
		static grab(ImageOutput) object = new ImageOutput();
		return object;
	}

	grab(MergeStage) ImageOutput::getMergeState()
	{
		return mMerge;
	}

	class TraverseAddImage : public IDirectoryTraverse
	{
	public:
		grab(MergeStage) mMerge;
		Array<more(char)> &mPaths;
		TraverseAddImage(grab(MergeStage) merge, Array<more(char)> &paths)
			: mMerge(merge), mPaths(paths)
		{}
		virtual bool dealWithDirectory(const Path &path)
		{
			mPaths.insert(path.getChars());
			mMerge->addImage(path.getChars().get());
			return true;
		}
	};

	void ImageOutput::addImage(const char *path)
	{
		Path p(path);
		if (p.isFolder()){
			TraverseAddImage a(mMerge, mPaths);
			grab(Directory) dir = Directory::create(p);
			dir->enumTraverse(interf_make_nop(IDirectoryTraverse)(&a));
		}else{
			mPaths.insert(Utils::copyStr(path));
			mMerge->addImage(path);
		}
	}

	void ImageOutput::setImagePath(const char *path)
	{
		mImagePath = Utils::copyStr(path);
	}

	void ImageOutput::setPlistPath(const char *path)
	{
		mPlistPath = Utils::copyStr(path);
	}

	bool ImageOutput::executeMerge()
	{
		if (mPaths.count() > 0 && mMerge->merge()){
			mMerge->makeImage(mImagePath.get());
			ImageInfo info;
			for (int i = 0; i<mPaths.count(); i++){
				if (mMerge->getImageData(mPaths[i], info)){
					
				}
			}
			return true;
		}
		return false;
	}

	ImageOutput::ImageOutput()
	{
		mMerge = MergeStage::create();
		mImagePath = Utils::copyStr("out.png");
		mPlistPath = Utils::copyStr("out.plist");
	}

	ImageOutput::~ImageOutput()
	{}
	
}
