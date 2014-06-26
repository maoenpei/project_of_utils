/***************************************

'Buffer' class is used for memory allocation or deallocation.

The difference between Buffer and Array is:
1.Array contains values that already exist, Buffer pre-alloc memory and then fill in values.
2.template arg T of Buffer class must be simple type that can use memcpy to copy value of it

***************************************/
#ifndef __NARAN_BUFFER_H__
#define __NARAN_BUFFER_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"
#include "allocator/CAllocator.h"

NS_DEF_NARAN{

	template<class T>
	class CLS_EXPORT Buffer
	{
		T *mArr;
		int mNum;
		int mMax;
	public:
		inline Buffer() : mNum(0), mMax(4) {
			mArr = (T *)Allocator::allocShare(mMax * sizeof(T));
		}
		inline Buffer(int max) : mNum(0), mMax(max){
			mArr = (T *)Allocator::allocShare(mMax * sizeof(T));
		}
		inline Buffer(const Buffer &copy) : mNum(copy.mNum), mMax(copy.mMax){
			mArr = (T *)Allocator::allocShare(mMax * sizeof(T));
			memcpy(mArr, copy.mArr, mMax * sizeof(T));
		}
		inline Buffer &operator=(const Buffer &copy){
			mNum = copy.mNum;
			mMax = copy.mMax;
			mArr = (T *)Allocator::allocShare(mMax * sizeof(T));
			memcpy(mArr, copy.mArr, mMax * sizeof(T));
			return *this;
		}
		inline ~Buffer(){
			Allocator::freeShare(mArr);
		}
		inline T &operator[](int index){
			assert(index >= 0 && index < mNum);
			return mArr[index];
		}
		inline void reset(){
			mNum = 0;
		}
		inline T *want(int offset, int size){
			assert(offset >= 0 && offset <= mNum && size >= 0);
			if (mNum + size > mMax){
				int newMax = mMax * 2 + size;
				T *newArr = (T *)Allocator::allocShare(newMax * sizeof(T));
				if (offset > 0){
					memcpy(newArr, mArr, offset * sizeof(T));
				}
				if (mNum > offset){
					memcpy(newArr + offset + size, mArr + offset, (mNum - offset) * sizeof(T));
				}
				Allocator::freeShare(mArr);
				mArr = newArr;
				mMax = newMax;
			}else{
				memmove(mArr + offset + size, mArr + offset, size * sizeof(T));
			}
			mNum += size;
			return mArr + offset;
		}
		inline T *want_tail(int size){
			return want(mNum, size);
		}
		inline T *want_front(int size){
			return want(0, size);
		}
		inline void limit(int size){
			assert(size >= 0 && size <= mNum);
			mNum = size;
		}
		inline void limit(T *l){
			limit(l - mArr);
		}
		inline int count(){
			return mNum;
		}
		
		inline arr(T) toArr(){
			T *objs = new T [(mNum < 1 ? 1 : mNum)] ();
			memcpy(objs, mArr, mNum * sizeof(T));
			arr(T) object(objs, mNum);
			return object;
		}
	};

}

#endif

