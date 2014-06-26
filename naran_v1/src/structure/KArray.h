#ifndef __NARAN_ARRAY_H__
#define __NARAN_ARRAY_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"
#include "allocator/CAllocator.h"

NS_DEF_NARAN{

	template<typename T>
	class CLS_EXPORT Array
	{
		T *mArr;
		int mNum;
		int mMax;
	public:
		inline Array() : mNum(0), mMax(4) {
			mArr = (T *)Allocator::allocShare(mMax * sizeof(T));
		}
		inline Array(const Array &copy) : mNum(copy.mNum), mMax(copy.mMax){
			mArr = (T *)Allocator::allocShare(mMax * sizeof(T));
			for (int i = 0; i<mNum; i++)
				new (mArr + i) T (copy.mArr[i]);
		}
		inline Array &operator=(const Array &copy){
			mNum = copy.mNum;
			mMax = copy.mMax;
			mArr = (T *)Allocator::allocShare(mMax * sizeof(T));
			for (int i = 0; i<mNum; i++)
				new (mArr + i) T (copy.mArr[i]);
			return *this;
		}
		inline ~Array(){
			for (int i = 0; i<mNum; i++)
				mArr[i].~T();
			Allocator::freeShare(mArr);
		}
		inline const T &getAt(int index) const{
			if (index < 0){
				index = mNum+index;
			}
			assert(index >= 0 && index < mNum);
			return mArr[index];
		}
		inline T &operator[](int index){
			if (index < 0){
				index = mNum+index;
			}
			assert(index >= 0 && index < mNum);
			return mArr[index];
		}
		inline void reset(){
			for (int i = 0; i<mNum; i++)
				mArr[i].~T();
			mNum = 0;
		}
		inline void insert(const T &item, int index = -1){
			if (mNum == mMax){
				T *new_arr = (T *)Allocator::allocShare(mMax * 2 * sizeof(T));
				memcpy(new_arr, mArr, mMax * sizeof(T));
				Allocator::freeShare(mArr);
				mArr = new_arr;
				mMax *= 2;
			}
			if (index < 0){
				index = mNum+1+index;
			}
			assert(index >= 0 && index <= mNum);
			memmove(mArr + index + 1, mArr + index, (mNum - index) * sizeof(T));
			new (mArr + index) T (item);
			mNum++;
		}
		inline void remove(int index){
			if (index < 0){
				index = mNum+index;
			}
			assert(index >= 0 && index < mNum);
			mArr[index].~T();
			memmove(mArr+index, mArr+index+1, (mNum - index - 1) * sizeof(T));
			mNum--;
		}
		inline int count(){return mNum;}

		// T type must override or use default 'operator ='
		inline arr(T) toArr(){
			T *objs = new T [(mNum < 1 ? 1 : mNum)] ();
			for (int i = 0; i<mNum; i++){
				objs[i] = mArr[i];
			}
			arr(T) object(objs, mNum);
			return object;
		}
	};

}

#endif

