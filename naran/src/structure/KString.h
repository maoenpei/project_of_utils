#ifndef __NARAN_STRING_H__
#define __NARAN_STRING_H__

#include "core/CommonDef.h"
#include "core/AutoReference.h"
#include "core/Utils.h"

NS_DEF_NARAN{

	class CLS_EXPORT String
	{
		u32 mHash;
		more(char) mStr;
	public:
		inline String() : mStr(nullof(char)), mHash(0){}
		inline String(c_str str) : mStr(Utils::copyStr(str)), mHash(Utils::hashStr(str)) {}
		inline String(const String&copy) : mStr(copy.mStr), mHash(copy.mHash) {}
		inline String &operator = (const String &copy){
			mStr = copy.mStr;
			mHash = copy.mHash;
			return *this;
		}
		inline bool operator ==(const String &other) const{
			return (mHash == other.mHash && (mStr.get() == other.mStr.get() || 0 == strcmp(mStr.get(), other.mStr.get())));
		}
		inline bool operator !=(const String &other) const{
			return ! operator ==(other);
		}
		inline operator bool (){
			return mStr && mStr[0] != 0;
		}
		inline more(char) getStr() const{
			return mStr;
		}
	};

}

#endif

