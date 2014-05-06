

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
	}

	void AtlasMerge::setImage(c_str name, grab(Image) img)
	{
		int index = indexOfName(name);
		if (index < 0){
			AtlasImageData *data = new AtlasImageData;
			data->img = img;
			data->name = Utils::copyStr(name);
			data->nameHash = Utils::hashStr(name);
			mImages.append(data);
			setImageDirty();
		}
	}

	void AtlasMerge::removeImage(c_str name)
	{
		int index = indexOfName(name);
		if (index >= 0){
			mImages.remove(index);
		}
	}

	grab(Image) AtlasMerge::getImage(c_str name)
	{
		int index = indexOfName(name);
		if (index >= 0){
			return mImages[index]->img;
		}
		return nullof(Image);
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

	int AtlasMerge::indexOfName(c_str name)
	{
		u32 nameHash = Utils::hashStr(name);
		for (int i = 0; i<mImages.length(); i++){
			if (nameHash == mImages[i]->nameHash
				&& 0 == strcmp(name, mImages[i]->name)){
				return i;
			}
		}
		return -1;
	}

	void AtlasMerge::getValidRegion(grab(AtlasImageData) imgData)
	{
	}


}



