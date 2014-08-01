/***************************************

'Data' class is used for memory record.

It didn't expand or free data, but it record and convert them.

***************************************/
#ifndef __NARAN_DATA_H__
#define __NARAN_DATA_H__

#include "CommonDef.h"

NS_DEF_NARAN{

	class CLS_EXPORT Data
	{
		void *mPtr;
		int mL;
	public:
		inline Data() : mPtr(NULL), mL(0) {}
		inline Data(void *ptr, int l) : mPtr(ptr), mL(l) {}
		inline Data(const Data &copy) : mPtr(copy.mPtr), mL(copy.mL) {}
		inline Data &operator =(const Data &copy){
			mPtr = copy.mPtr;
			mL = copy.mL;
			return *this;
		}
		inline void reset(const Data &copy){
			mPtr = copy.mPtr;
			mL = copy.mL;
		}

		inline int l() const{
			return mL;
		}
		inline void *ptr() const{
			return mPtr;
		}
		inline operator bool() const{
			return mPtr != NULL;
		}
		template<class T>
		inline T get() const{
			assert(mPtr && mL >= sizeof(T));
			return *((T *)mPtr);
		}
		template<class T>
		inline T get(int index) const{
			assert(mPtr && index >= 0 && mL >= sizeof(T) * (index + 1));
			return ((T *)mPtr)[index];
		}
		template<class T>
		inline void set(const T &val){
			assert(mPtr && mL >= sizeof(T));
			*((T *)mPtr) = val;
		}
		template<class T>
		inline void set(const T &val, int index){
			assert(mPtr && index >= 0 && mL >= sizeof(T) * (index + 1));
			((T *)mPtr)[index] = val;
		}
	};

}

#endif

