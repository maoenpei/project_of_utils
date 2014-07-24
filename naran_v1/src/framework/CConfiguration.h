#ifndef __NARAN_CONFIGURATION_H__
#define __NARAN_CONFIGURATION_H__

#include "CommonDef.h"
#include "autoref/AutoReference.h"
#include "structure/KData.h"

NS_DEF_NARAN{

	class CLS_EXPORT Configuration
	{
	public:
		int getDataLength(const char *key);
		bool getData(const char *key, Data &dat);
		bool setData(const char *key, const Data &dat);

		template<class T>
		bool get(const char *key, T &dat){
			Data dats(&dat, sizeof(T));
			return getData(key, dats);
		}

		template<class T>
		bool set(const char *key, const T &dat){
			Data dats((void *)&dat, sizeof(T));
			return setData(key, dats);
		}

		static grab(Configuration) create();

	private:
		void *mPtr;
		
		CLS_HIDE(Configuration);
	};

}


#endif

