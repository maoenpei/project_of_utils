

#include "ImageRectManager.h"


NS_DEF_NARAN{

	static grab(Image) changeToRGBA(grab(Image) img)
	{
		more(byte) data = img->getData();
		int channel = img->getChannel();
		const g2d::Sizei &size = img->getSize();
		
		if (channel == 4){
			return img;
		}
		if (channel == 3){
			int channel2 = 4;
			more(byte) data2 = new byte[channel2 * (size.w * size.h)];
			
			byte *data_from = data.get();
			byte *data_to = data2.get();
			for (int i = size.w * size.h - 1; i>=0; i--){
				data_to[0] = data_from[0];
				data_to[1] = data_from[1];
				data_to[2] = data_from[2];
				data_to[3] = 0xff;
				data_to += 4;
				data_from += 3;
			}
			return Image::create(data2, size, channel2);
		}
		return nullof(Image);
	}

	static void computeValidRect(g2d::Recti *rect, grab(Image) img)
	{
		more(byte) data = img->getData();
		int channel = img->getChannel();
		const g2d::Sizei &size = img->getSize();
		
		int l = size.w, r = 0, t = size.h, b = 0;
		
		unsigned int *line = (unsigned int *)data.get();
		for (int j = 0; j<size.h; j++){
			bool lineClean = true;
			for (int i = 0; i<size.w; i++){
				if (*line){
					lineClean = false;
					if (i<l){
						l = i;
					}
					if (i+1>r){
						r = i+1;
					}
				}
				line++;
			}
			if (!lineClean){
				if (j<t){
					t = j;
				}
				if (j+1>b){
					b = j+1;
				}
			}
		}
		if (l > r || b > t){
			l = 0;
			r = 1;
			t = 0;
			b = 1;
		}
		
		rect->pt = g2d::Pointi(l, t);
		rect->size = g2d::Sizei(r-l, b-t);
	}

	bool ImageRectManager::addImageRect(ImageEntry *entry, const char *path)
	{
		Path p(path);
		more(char) extName = p.getExtName();
		grab(ImageLoader) loader = ImageLoader::sharedLoader(extName.get());
		if (! loader){
			return false;
		}
		entry->image = changeToRGBA(loader->decodeImage(path));
		if (! entry->image){
			return false;
		}
		computeValidRect(&entry->region, entry->image);
		if (!entry->image->inRegion(entry->region)){
			return false;
		}
		grab(Image) cutImage = entry->image;
		if (entry->region.size != entry->image->getSize()){
			cutImage = entry->image->subImage(entry->region);
		}
		for (int i = mRects.count()-1; i>=0; i--){
			if (mRects[i]->cutImage->compareImage(cutImage)){
				entry->rect = mRects[i];
				return true;
			}
		}
		grab(ImageRectMergeStorage) imgRect = new ImageRectMergeStorage();
		imgRect->cutImage = cutImage;
		imgRect->size = cutImage->getSize();
		imgRect->markUsed = false;
		mRects.insert(imgRect);
		entry->rect = imgRect;
		return true;
	}

	void ImageRectManager::reset()
	{
		mRects.reset();
	}

	arr(ImageRectMergeStorage *) ImageRectManager::getRects()
	{
		return mRects.toArr().convert_force<ImageRectMergeStorage *>();
	}

	grab(ImageRectManager) ImageRectManager::create()
	{
		return new ImageRectManager();
	}

	ImageRectManager::ImageRectManager()
	{}

	ImageRectManager::~ImageRectManager()
	{}


}

