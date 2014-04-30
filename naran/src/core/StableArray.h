#ifndef __NARAN_STABLE_ARRAY_H__
#define __NARAN_STABLE_ARRAY_H__

#include "core/CommonDef.h"
#include "core/AutoCounter.h"
#include <new>

NS_DEF_NARAN{

	template<typename I>
	class StableArray
	{
		stable(I) *mArr;
		int mNum;
		int mMax;
	public:
		inline StableArray() : mNum(0), mMax(4) {
			mArr = (stable(I) *)malloc(mMax * sizeof(stable(I)));
		}
		inline StableArray(const StableArray &copy) : mNum(copy.mNum), mMax(copy.mMax){
			mArr = (stable(I) *)malloc(mMax * sizeof(stable(I)));
			for (int i = 0; i<mNum; i++)
				new (mArr + i) stable(I) (copy.mArr[i]);
		}
		inline StableArray &operator=(const StableArray &copy){
			mNum = copy.mNum;
			mMax = copy.mMax;
			mArr = (stable(I) *)malloc(mMax * sizeof(stable(I)));
			for (int i = 0; i<mNum; i++)
				new (mArr + i) stable(I) (copy.mArr[i]);
			return *this;
		}
		inline ~StableArray(){
			for (int i = 0; i<mNum; i++)
				mArr[i].~Closure_();
			free(mArr);
		}
		inline stable(I) &operator[](int index){
			assert(index >= 0 && index < mNum);
			return mArr[index];
		}
		inline void append(const stable(I) &item, int index){
			if (mNum == mMax){
				stable(I) *new_arr = (stable(I) *)malloc(mMax * 2 * sizeof(stable(I)));
				memcpy(new_arr, mArr, mMax * sizeof(stable(I)));
				free(mArr);
				mArr = new_arr;
				mMax *= 2;
			}
			if (index < 0){
				index = mNum+1+index;
			}
			assert(index >= 0 && index <= mNum);
			memmove(mArr + index + 1, mArr + index, (mNum - index) * sizeof(stable(I)));
			new (mArr + index) stable(I) (item);
			mNum++;
		}
		inline void remove(int index){
			assert(index >= 0 && index < mNum);
			mArr[index].~Closure_();
			memmove(mArr+index, mArr+index+1, (mNum - index - 1) * sizeof(stable(I)));
			mNum--;
		}
		inline int count(){return mNum;}
	};

#define arr_stable(CLS)			StableArray<CLS>

}

#endif

