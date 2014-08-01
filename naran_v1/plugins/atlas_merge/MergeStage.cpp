

#include "MergeStage.h"


NS_DEF_NARAN{

	bool MergeStage::merge()
	{
		mRects.reset();
		return true;
	}

	bool MergeStage::makeImage(const char *path)
	{
		if (mRects.count() <= 0){
			return false;
		}
		return true;
	}

	void MergeStage::addImage(const char *path)
	{
		grab(ImageEntry) entry = new ImageEntry();
		
		entry->path = Utils::copyStr(path);
		Path p(entry->path);
		more(char) extName = p.getExtName();
		grab(ImageLoader) loader = ImageLoader::sharedLoader(extName.get());
		if (! loader){
			return;
		}
		entry->image = loader->decodeImage(path);
		if (! entry->image){
			return;
		}
		entry->rect = NULL;
		
		mEntrys.set(path, entry);
	}

	void MergeStage::removeImage(const char *path)
	{
		mEntrys.remove(path);
	}

	bool MergeStage::getImageData(const char *path, ImageInfo &info)
	{
		if (mRects.count() <= 0){
			return false;
		}
		return true;
	}

	void MergeStage::setTrim(bool isTrim)
	{
		mConf->set("trim", isTrim);
	}

	void MergeStage::setBorderPadding(int padding)
	{
		mConf->set("borderpadding", padding);
	}

	void MergeStage::setInnerPadding(int padding)
	{
		mConf->set("innerpadding", padding);
	}

	void MergeStage::setShape(u32 shape)
	{
		mConf->set("shape", shape);
	}

	void MergeStage::setSquare(bool isSquare)
	{
		mConf->set("square", isSquare);
	}

	void MergeStage::setAllowRotation(bool isAllowRotation)
	{
		mConf->set("allowrotation", isAllowRotation);
	}

	grab(MergeStage) MergeStage::create()
	{
		return new MergeStage();
	}

	MergeStage::MergeStage()
	{
		mConf = Configuration::create();
		setTrim(true);
		setBorderPadding(1);
		setInnerPadding(1);
		setShape((u32)ImageShape_Any);
		setSquare(false);
		setAllowRotation(true);
	}

	MergeStage::~MergeStage()
	{
	}

}
