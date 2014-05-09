

#include "AtlasMerge.h"

CLS_SHARE_MEMORY();

NS_DEF_NARAN{

	void AtlasMerge::setImageDirty()
	{
		mImageDirty = true;
		setOrderDirty();
	}

	void AtlasMerge::setOrderDirty()
	{
		mOrderedImages = NULL;
		setMergeDirty();
	}

	void AtlasMerge::setMergeDirty()
	{
		mMergeDirty = true;
		mBlockBelongs.drop();
	}

	void AtlasMerge::setImage(c_str name, grab(Image) img)
	{
		assert(img->getWidth() > 0 && img->getHeight() > 0);
		String sName(name);
		int index = indexOfName(sName);
		if (index < 0){
			AtlasImageData *data = new AtlasImageData(sName, img);
			mImages.append(data);
			setImageDirty();
		}
	}

	void AtlasMerge::removeImage(c_str name)
	{
		String sName(name);
		int index = indexOfName(sName);
		if (index >= 0){
			mImages.remove(index);
			setOrderDirty();
		}
	}

	grab(Image) AtlasMerge::getImage(c_str name)
	{
		String sName(name);
		int index = indexOfName(sName);
		if (index >= 0){
			return mImages[index]->img;
		}
		return nullof(Image);
	}

	void AtlasMerge::setTrim(bool istrim)
	{
		if (! mIsTrim != ! istrim){
			mIsTrim = istrim;
			setImageDirty();
		}
	}

	void AtlasMerge::clean()
	{
		if (mImageDirty){
			mImageDirty = false;
			for (int i = 0; i<mImages.length(); i++){
				getValidRegion(mImages[i]);
				mImages[i]->block = nullof(AtlasBlockData);
				for (int j = 0; j<i; j++){
					if (isRegionEqual(mImages[i], mImages[j])){
						mImages[i]->block = mImages[j]->block;
						break;
					}
				}
				if (! mImages[i]->block){
					createBlock(mImages[i]);
				}
			}
		}
	}

	int AtlasMerge::indexOfName(const String &sName)
	{
		for (int i = 0; i<mImages.length(); i++){
			if (sName == mImages[i]->name){
				return i;
			}
		}
		return -1;
	}

	void AtlasMerge::getValidRegion(grab(AtlasImageData) imgData)
	{
		grab(Image) img = imgData->img;
		int width = img->getWidth();
		int height = img->getHeight();
		if (! mIsTrim || ! img->getAlpha()){
			imgData->imgcut = g2d::Recti(0, 0, width, height);
		}else{
			int top = -1;
			int bottom = 0;
			int left = width;
			int right = 0;
			int rowbytes = width * img->getChannels();
			channel * data = img->getData();
			more(channel) zero = new channel[rowbytes] ();
			for (int i = 0; i<height; i++){
				if (0 != memcmp(zero.get(), data, rowbytes)){
					// row check
					top = (top == -1 ? i : top);
					bottom = i+1;
					// channel check
					pixel *line = (pixel *)data;
					int tleft = -1;
					int tright = 0;
					for (int j = 0; j<width; j++){
						if (0 != *line){
							tleft = (tleft == -1 ? j : tleft);
							tright = j+1;
						}
					}
					left = MATH_MIN(left, tleft);
					right = MATH_MAX(right, tright);
				}
				data += rowbytes;
			}
			if (top == -1){
				top = 0;
				bottom = 1;
				left = 0;
				right = 1;
			}
			imgData->imgcut = g2d::Recti(left, top, right - left, bottom - top);
		}
	}

	bool AtlasMerge::isRegionEqual(grab(AtlasImageData) imgData, grab(AtlasImageData) imgData2)
	{
		g2d::Recti *imgcut = &imgData->imgcut;
		g2d::Recti *imgcut2 = &imgData2->imgcut;
		if (imgcut->size != imgcut2->size){
			return false;
		}
		grab(Image) img = imgData->img;
		grab(Image) img2 = imgData2->img;
		if (img->getChannels() != img2->getChannels()){
			return false;
		}
		int rowbytes = img->getWidth() * img->getChannels();
		int rowbytes2 = img2->getWidth() * img2->getChannels();
		channel *pstart = img->getData() + rowbytes * imgcut->pt.y + img->getChannels() * imgcut->pt.x;
		channel *pstart2 = img2->getData() + rowbytes2 * imgcut2->pt.y + img2->getChannels() * imgcut2->pt.x;
		int cmpSize = imgcut->size.w * img->getChannels();
		for (int i = imgcut->size.h/2; i<imgcut->size.h; i++){
			if (0 != memcmp(pstart, pstart2, cmpSize)){
				return false;
			}
			pstart += rowbytes;
			pstart2 += rowbytes2;
		}
		return true;
	}

	void AtlasMerge::createBlock(grab(AtlasImageData) imgData)
	{
		AtlasBlockData *block = new AtlasBlockData;
		block->size = imgData->imgcut.size;
		imgData->block = block;
	}

	void AtlasMerge::resetOrder()
	{
		for (int i = 0; i<mImages.length(); i++){
			mImages[i]->after = "";
		}
		setOrderDirty();
	}

	void AtlasMerge::orderAfter(c_str name, c_str other)
	{
		String sName(name);
		int index = indexOfName(sName);
		if (index >= 0){
			mImages[index]->after = other;
		}
		setOrderDirty();
	}

	void AtlasMerge::setOrderType(u32 type, bool revert)
	{
		assert(type >= 0 && type < AtlasOrder_Max);
		if (mOrderType != type || ! mIsRevert != ! revert){
			mOrderType = type;
			mIsRevert = revert;
			setOrderDirty();
		}
	}

	void AtlasMerge::sort()
	{
		int total = mImages.length();
		assert(total > 0);
		if (! mOrderedImages){
			clean();
			arr(AtlasImageData *) datas(total);
			for (int i = 0; i<total; i++){
				datas[i] = mImages[i].get();
			}
			datas = mergeSort(datas);
			linkImages(datas);
		}
	}

	arr(AtlasMerge::AtlasImageData *) AtlasMerge::mergeSort(arr(AtlasMerge::AtlasImageData *) datas)
	{
		arr(AtlasImageData *) copyto(datas.size());
		arr(AtlasImageData *) temp;
		int total = datas.size();
		int unit = 1;
		while(unit < total){
			for (int i = 0; i<total; i += unit * 2){
				// sort [i ~ i+unit-1] [i+unit ~ i+2*unit-1]
				if (i + unit >= total){
					memcpy(copyto.get() + i, datas.get() + i, (total - i) * sizeof(AtlasImageData *));
					continue;
				}
				int k = i;
				int j = i+unit;
				int limit = (i + 2 * unit < total ? i + 2 * unit : total);
				int n = i;
				while(k < i + unit && j < limit){
					if (smallerImages(datas[k], datas[j])){
						copyto[n++] = datas[j++];
					}else{
						copyto[n++] = datas[k++];
					}
				}
				if (k < i+unit){
					memcpy(copyto.get() + n, datas.get() + k, (limit - n) * sizeof(AtlasImageData *));
				}else if (j < limit){
					memcpy(copyto.get() + n, datas.get() + j, (limit - n) * sizeof(AtlasImageData *));
				}
			}
			temp = copyto;
			copyto = datas;
			datas = temp;
			unit *= 2;
		}
		return copyto;
	}

	bool AtlasMerge::smallerImages(AtlasMerge::AtlasImageData *data, AtlasMerge::AtlasImageData *data2)
	{
		AtlasImageData *pdata = data;
		while(pdata->after){
			int index = indexOfName(pdata->after);
			if (index < 0)
				break;
			pdata = mImages[index].get();
			if (pdata == data2){
				return true;
			}
		}
		pdata = data2;
		while(pdata->after){
			int index = indexOfName(pdata->after);
			if (index < 0)
				break;
			pdata = mImages[index].get();
			if (pdata == data){
				return false;
			}
		}
		switch(mOrderType)
		{
		case AtlasOrder_Name:
			{
				return (! mIsRevert 
					? strcmp(data->name.getStr().get(), data2->name.getStr().get()) < 0
					: strcmp(data->name.getStr().get(), data2->name.getStr().get()) > 0);
			}
		case AtlasOrder_Area:
			{
				int area = data->imgcut.size.w * data->imgcut.size.h;
				int area2 = data2->imgcut.size.w * data2->imgcut.size.h;
				return (! mIsRevert
					? area < area2
					: area > area2);
			}
		case AtlasOrder_Width:
			{
				int val = data->imgcut.size.w;
				int val2 = data2->imgcut.size.w;
				return (! mIsRevert
					? val < val2
					: val > val2);
			}
		case AtlasOrder_Height:
			{
				int val = data->imgcut.size.h;
				int val2 = data2->imgcut.size.h;
				return (! mIsRevert
					? val < val2
					: val > val2);
			}
		}
		return false;
	}

	void AtlasMerge::linkImages(arr(AtlasMerge::AtlasImageData *) datas)
	{
		AtlasImageData **refData = &mOrderedImages;
		for (int i = 0; i<datas.size(); i++){
			*refData = datas[i];
			refData = &datas[i]->next;
		}
		*refData = NULL;
	}

	void AtlasMerge::merge()
	{
		if (mMergeDirty){
			sort();
			for (AtlasImageData *first = mOrderedImages; first; first = first->next){
				first->block->marked = 0;
			}
			Array<AtlasImageData *> blocks;
			Array<g2d::Sizei *> sizes;
			Array<g2d::Recti *> rects;
			for (AtlasImageData *first = mOrderedImages; first; first = first->next){
				if (first->block->marked == 0){
					sizes.append(& first->block->size);
					rects.append(& first->block->atlascut);
					blocks.append((AtlasImageData *const)first->block.get());
					memset(& first->block->atlascut, 0, sizeof(g2d::Recti));
				}
				first->block->marked++;
			}
			memset(&mAtlasSize, 0, sizeof(mAtlasSize));
			mMergeSuccess = mBounding->mergeImage(sizes.toArr(), rects.toArr(), mAtlasSize);
			mMergeDirty = false;
			mBlockBelongs = blocks.toArr();
		}
	}

	grab(AtlasInfo) AtlasMerge::getInfo(c_str name)
	{
		if (mMergeDirty){
			return nullof(AtlasInfo);
		}
		String sName(name);
		int index = indexOfName(sName);
		if (index){
			AtlasInfo *info = new AtlasInfo();
			atlasInfoFill(mImages[index].get(), info);
			return info;
		}
		return nullof(AtlasInfo);
	}

	arr(AtlasInfo) AtlasMerge::getAllInfo()
	{
		if (mMergeDirty){
			return arr(AtlasInfo)();
		}
		int count = mImages.length();
		arr(AtlasInfo) infos(count);
		for (int i = 0; i<count; i++){
			atlasInfoFill(mImages[i].get(), &infos[i]);
		}
		return infos;
	}

	arr(more(char)) AtlasMerge::getMergeInfo(c_str name)
	{
		if (mMergeDirty){
			return arr(more(char))();
		}
		String sName(name);
		int index = indexOfName(sName);
		if (index >= 0){
			int count = mImages[index]->block->marked;
			arr(more(char)) infos(count);
			for (int i = 0; i<mImages.length(); i++){
				if (mImages[i]->block.get() == mImages[index]->block.get()){
					infos[i] = mImages[i]->name.getStr();
				}
			}
			return infos;
		}
		return arr(more(char))();
	}

	grab(Image) AtlasMerge::genAtlas()
	{
		if (mMergeDirty){
			return nullof(Image);
		}
		grab(Image) image = Image::create();
		image->setAlpha(true);
		image->resize(mAtlasSize);
		for (int i = 0; i<mBlockBelongs.size(); i++){
			atlasFill(image, mBlockBelongs[i]);
		}
		return image;
	}

	void AtlasMerge::atlasInfoFill(AtlasImageData *img, AtlasInfo *info)
	{
		g2d::Recti *pImgCut = &img->imgcut;
		g2d::Recti *pAtlasCut = &img->block->atlascut;
		info->name = img->name.getStr();
		info->imgcut = *pImgCut;
		info->offset = pAtlasCut->pt;
		info->rotated = pImgCut->size != pAtlasCut->size;
	}

	void AtlasMerge::atlasFill(grab(Image) img, AtlasMerge::AtlasImageData *data)
	{
		g2d::Recti rectSource = data->imgcut;
		g2d::Recti rectTarget = data->block->atlascut;
		if (rectTarget.size.w <= 0 || rectTarget.size.h <= 0){
			return ;
		}
		bool rotated = rectSource.size != rectTarget.size;
		grab(Image) imgSource = data->img->getSubImage(rectSource);
		if (rotated){
			imgSource->rotateImage();
		}
		img->setSubImage(rectTarget.pt, imgSource);
	}

	grab(AtlasMerge) AtlasMerge::create()
	{
		grab(AtlasMerge) object(new AtlasMerge());
		return object;
	}

	AtlasMerge::AtlasMerge()
		: mIsTrim(true)
		, mIsRevert(false)
		, mOrderType(AtlasOrder_Name)
		, mBounding(AtlasBounding::create())
	{}

	AtlasMerge::~AtlasMerge()
	{}


}



