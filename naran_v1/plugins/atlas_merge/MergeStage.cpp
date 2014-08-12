

#include "MergeStage.h"
#include "SortAlgorithm.h"
#include "MergeAlgorithm.h"

NS_DEF_NARAN{

	class SortByWidth : public MergeSortAlgorithm{
		virtual bool compareGreaterThan(ImageRect *rect1, ImageRect *rect2){
			return rect1->size.w > rect2->size.w;
		}
	};
	class SortByHeight : public MergeSortAlgorithm{
		virtual bool compareGreaterThan(ImageRect *rect1, ImageRect *rect2){
			return rect1->size.h > rect2->size.h;
		}
	};
	class SortByArea : public MergeSortAlgorithm{
		virtual bool compareGreaterThan(ImageRect *rect1, ImageRect *rect2){
			return rect1->size.w * rect1->size.h > rect2->size.w * rect2->size.h;
		}
	};
	class SortByLength : public MergeSortAlgorithm{
		virtual bool compareGreaterThan(ImageRect *rect1, ImageRect *rect2){
			return (rect1->size.w > rect1->size.h ? rect1->size.w : rect1->size.h)
				> (rect2->size.w > rect2->size.h ? rect2->size.w : rect2->size.h);
		}
	};

	static grab(MergeSortAlgorithm) getAlgorithm(u32 sort){
		switch(sort){
			case ImageSort_Width:
				return new SortByWidth();
			case ImageSort_Height:
				return new SortByHeight();
			case ImageSort_Area:
				return new SortByArea();
			case ImageSort_Length:
				return new SortByLength();
		}
		return nullof(MergeSortAlgorithm);
	}

	bool MergeStage::merge()
	{
		if (mImageDirty){
			// add manage
			mManager->reset();
			StringMap(grab(ImageEntry))::IterType it(mEntrys.iterator());
			while(it.hasNext){
				if (!mManager->addImageRect(it.value.get(), it.key)){
					return false;
				}
				it.next();
			}

			arr(ImageRectMergeStorage *) rects = mManager->getRects();
			
			// sort
			arr(ImageRect *) rects2;
			u32 sort = ImageSort_None;
			mConf->get("sort", sort);
			grab(MergeSortAlgorithm) msort = getAlgorithm(sort);
			if (msort){
				ISortAlgorithm *sorter = msort.get();
				rects2 = sorter->sort(rects.convert_static<ImageRect *>());
			}else{
				rects2 = rects.convert_static<ImageRect *>();
			}
			
			// merge
			grab(MergeAlgorithm) mmerger = new MergeAlgorithm();
			IMergeAlgorithm *merger = mmerger.get();
			g2d::Sizei totalSize;
			int count = merger->merge(rects2.convert_static<ImageRectMerge *>(), mConf, totalSize);
			mConf->set("r_count", count);
			mConf->set("r_totalsize", totalSize);
			
			mImageDirty = false;
		}
		return true;
	}

	bool MergeStage::makeImage(const char *path)
	{
		if (mImageDirty){
			return false;
		}
		
		int count;
		mConf->get("r_count", count);
		
		int channel = 4;
		g2d::Sizei size;
		mConf->get("r_totalsize", size);
		more(byte) data = new byte[channel * (size.w * size.h)]();
		grab(Image) img = Image::create(data, size, 4);

		arr(ImageRectMergeStorage *) rects = mManager->getRects();
		for (int i = 0; i<count; i++){
			grab(Image) zimage = rects[i]->cutImage;
			if (zimage->getSize() != rects[i]->rect.size){
				zimage = zimage->rotateImage(1);
			}
			if (zimage->getSize() != rects[i]->rect.size){
				return false;
			}
			img->coverImage(zimage, rects[i]->rect.pt, rects[i]->rect.size);
		}

		return ImageLoader::sharedLoader(".png")->encodeImage(path, img);
	}

	bool MergeStage::getImageData(const char *path, ImageInfo &info)
	{
		if (mImageDirty){
			return false;
		}
		
		grab(ImageEntry) entry = mEntrys.get(path);
		if (!entry || entry->rect == NULL){
			return false;
		}
		
		ImageRectMergeStorage *rect = entry->rect;
		if (!rect->markUsed){
			return false;
		}
		
		info.region = entry->region;
		info.atlas_region = rect->rect;
		
		return true;
	}

	void MergeStage::addImage(const char *path)
	{
		mImageDirty = true;
		grab(ImageEntry) entry = new ImageEntry();
		mEntrys.set(path, entry);
	}

	void MergeStage::removeImage(const char *path)
	{
		mImageDirty = true;
		mEntrys.remove(path);
	}

	void MergeStage::setTrim(bool isTrim)
	{
		mImageDirty = true;
		mConf->set("trim", isTrim);
	}

	void MergeStage::setBorderPadding(int padding)
	{
		mImageDirty = true;
		mConf->set("borderpadding", padding);
	}

	void MergeStage::setInnerPadding(int padding)
	{
		mImageDirty = true;
		mConf->set("innerpadding", padding);
	}

	void MergeStage::setPOT(bool isPOT)
	{
		mImageDirty = true;
		mConf->set("pot", isPOT);
	}

	void MergeStage::setSquare(bool isSquare)
	{
		mImageDirty = true;
		mConf->set("square", isSquare);
	}

	void MergeStage::setAllowRotation(bool isAllowRotation)
	{
		mImageDirty = true;
		mConf->set("allowrotation", isAllowRotation);
	}

	void MergeStage::setSort(u32 sort)
	{
		mImageDirty = true;
		mConf->set("sort", sort);
	}

	void MergeStage::setMaxSize(const g2d::Sizei &size)
	{
		mImageDirty = true;
		g2d::Sizei siz(size);
		siz.w = (siz.w <= 0 ? 4096 : siz.w);
		siz.h = (siz.h <= 0 ? 4096 : siz.h);
		mConf->set("maxsize", siz);
	}

	void MergeStage::setMaxWidth(int width)
	{
		g2d::Sizei size;
		mConf->get("maxsize", size);
		size.w = width;
		setMaxSize(size);
	}

	void MergeStage::setMaxHeight(int height)
	{
		g2d::Sizei size;
		mConf->get("maxsize", size);
		size.h = height;
		setMaxSize(size);
	}

	grab(MergeStage) MergeStage::create()
	{
		return new MergeStage();
	}

	MergeStage::MergeStage()
		: mImageDirty(true)
	{
		mConf = Configuration::create();
		mManager = ImageRectManager::create();
		setTrim(true);
		setBorderPadding(1);
		setInnerPadding(1);
		setPOT(false);
		setSquare(false);
		setAllowRotation(true);
		setSort(ImageSort_None);
		setMaxSize(g2d::Sizei(4096, 4096));
	}

	MergeStage::~MergeStage()
	{
	}

}
