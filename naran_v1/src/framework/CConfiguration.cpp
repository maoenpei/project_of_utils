

#include "CConfiguration.h"
#include "structure/KBuffer.h"
#include "structure/KHashmap.h"
#include "structure/KBitmap.h"

#define mData		((Data_Configuration *)mPtr)

NS_DEF_NARAN{

	struct DataRecord
	{
		int l;
		int index;
		DataRecord(int _l, int _index) : l(_l), index(_index) {}
		DataRecord() : l(0), index(-1) {}
	};

	struct Data_Configuration
	{
		IntegerMap(grab(Blocks)) buffers;		// data, bitmap, size
		StringMap(DataRecord) datas;			// data record
	};

	int Configuration::getDataLength(const char *key)
	{
		if (mData->datas.hasKey(key)){
			DataRecord record = mData->datas.get(key);
			return record.l;
		}
		return -1;
	}

	bool Configuration::getData(const char *key, Data &dat)
	{
		if (dat){
			if (mData->datas.hasKey(key)){
				DataRecord record = mData->datas.get(key);
				if (record.l == dat.l()){
					grab(Blocks) blocks = mData->buffers.get(record.l);
					memcpy(dat.ptr(), blocks->getBlock(record.index), record.l);
					return true;
				}
			}
		}
		return false;
	}

	bool Configuration::setData(const char *key, const Data &dat)
	{
		if (dat){
			if (mData->datas.hasKey(key)){
				DataRecord record = mData->datas.get(key);
				grab(Blocks) blocks = mData->buffers.get(record.l);
				if (record.l == dat.l()){
					memcpy(blocks->getBlock(record.index), dat.ptr(), record.l);
					return true;
				}
				blocks->setBits(false, record.index);
			}
			grab(Blocks) blocks = mData->buffers.get(dat.l());
			if (! blocks){
				blocks = new Blocks(4, dat.l());
				mData->buffers.set(dat.l(), blocks);
			}
			int index = blocks->getFirstBit(false);
			if (index < 0){
				grab(Blocks) new_blocks = new Blocks(blocks->count() * 2, dat.l());
				new_blocks->copyFromOther(*blocks);
				blocks = new_blocks;
				mData->buffers.set(dat.l(), blocks);
				index = blocks->getFirstBit(false);
			}
			byte *ptr = blocks->getBlock(index);
			blocks->setBits(true, index);
			memcpy(ptr, dat.ptr(), dat.l());
			mData->datas.set(key, DataRecord(dat.l(), index));
		}
		return false;
	}

	grab(Configuration) Configuration::create()
	{
		return new Configuration();
	}

	Configuration::Configuration()
	{
		mPtr = new Data_Configuration();
	}

	Configuration::~Configuration()
	{
		delete mData;
	}

}

