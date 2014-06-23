

#include "KHashtable.h"

NS_DEF_NARAN{

#define INDEX_OF_EMPTY(index, pairs, max, hash)		\
	do {\
		(index) = (hash) % (max);\
		while((pairs)[(index)])\
			(index) = ((index) + 1 >= (max) ? 0 : (index) + 1);\
	} while(0)

#define mData					((Data_Hashmap *)mHashData)

	struct Data_Hashmap
	{
		IHashCompare *mCompare;
		HashPair **mPairs;
		int mNum;
		int mMax;
		inline Data_Hashmap() : mNum(0), mMax(31), mPairs(new HashPair *[31] ()){}
		inline ~Data_Hashmap(){
			delete [] mPairs;
		}
		inline void input(HashPair *pair){
			// expand capacity
			if (mNum >= mMax / 7){
				int newMax = mMax * 4 + 3;
				HashPair **newPairs = new HashPair *[newMax] ();
				for (int i = 0; i<mMax; i++){
					if (mPairs[i]){
						int index;
						INDEX_OF_EMPTY(index, newPairs, newMax, mPairs[i]->uHash);
						newPairs[index] = mPairs[i];
					}
				}
				mMax = newMax;
				delete [] mPairs;
				mPairs = newPairs;
			}
			int index;
			INDEX_OF_EMPTY(index, mPairs, mMax, pair->uHash);
			mPairs[index] = pair;
			mNum++;
		}
		inline HashPair *find(u32 hash, void *key){
			int index = hash % mMax;
			while(mPairs[index]){
				if (mPairs[index]->uHash == hash
					&& mCompare->compare(mPairs[index]->key, key)){
					return mPairs[index];
				}
				index = (index + 1 > mMax ? 0 : index + 1);
			}
			return NULL;
		}
		inline bool remove(HashKey *pair){
			int index = pair->uHash % mMax;
			while(mPairs[index]){
				if ((u32)mPairs[index] == (u32)pair){
					mPairs[index] = NULL;
					mNum--;
					return true;
				}
				index = (index + 1 > mMax ? 0 : index + 1);
			}
			return false;
		}
	};

	Hashtable::Hashtable(IHashCompare *compare)
		: mHashData(new Data_Hashmap())
	{
		mData->mCompare = compare;
	}

	Hashtable::~Hashtable()
	{
		HashPair **pairs = mData->mPairs;
		for (int i = mData->mMax - 1; i>=0; i--){
			delete pairs[i];
		}
		delete mData;
	}

	HashPair *Hashtable::findPair(u32 hash, void *key)
	{
		return mData->find(hash, key);
	}

	void Hashtable::addPair(u32 hash, void *key, void *value)
	{
		HashPair *pair = new HashPair();
		pair->uHash = hash;
		pair->key = key;
		pair->value = value;
		mData->input(pair);
	}

	void Hashtable::removePair(HashKey *pair)
	{
		if (mData->remove(pair)){
			delete pair;
		}
	}

	Hashiter * Hashtable::iterator()
	{
		Hashiter *object = new Hashiter();
		object->mPairs = mData->mPairs;
		object->mMax = mData->mMax;
		object->mNow = 0;
		return object;
	}

	HashPair *Hashiter::next()
	{
		for (; mNow < mMax; mNow++){
			if (mPairs[mNow]){
				return mPairs[mNow++];
			}
		}
		return NULL;
	}

}

