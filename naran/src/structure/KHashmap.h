#ifndef __NARAN_HASH_MAP_H__
#define __NARAN_HASH_MAP_H__

#include "core/CommonDef.h"
#include "core/Utils.h"

NS_DEF_NARAN{

	class ICompare
	{
	public:
		// compare inner key to outer key
		virtual bool compare(void *key1, void *key2) = 0;
	};

	struct CLS_EXPORT HashPair
	{
		u32 uHash;
		void *key;
		void *value;
	};

	class CLS_EXPORT Hashiter
	{
		friend class Hashtable;
		HashPair **mPairs;
		int mMax;
		int mNow;
	public:
		HashPair *next();
	};

	class CLS_EXPORT Hashtable
	{
	protected:
		Hashtable(ICompare *compare);
		~Hashtable();
		
		HashPair * findPair(u32 hash, void *key);
		void addPair(u32 hash, void *key, void *value);
		void removePair(HashPair *pair);
		grab(Hashiter) iterator();

	private:
		void *mHashData;
	};
	
	template<class HT, class HOP, class T>
	class CLS_EXPORT HHashiter
	{
		grab(Hashiter) mIter;
	public:
		HHashiter(grab(Hashiter) iter) : mIter(iter) {
			next();
		}
		HT key;
		T value;
		bool hasKey;
		void next(){
			HashPair *pair = mIter->next();
			hasKey = pair != NULL;
			if (pair){
				key = HOP::convert(pair->key);
				value = *((T *)pair->value);
			}
		}
	};

	template<class HT, class HOP, class T>
	class CLS_EXPORT HHashmap : public Hashtable
	{
		class CLS_EXPORT HCompare : public ICompare
		{
		public:
			virtual bool compare(void *key1, void *key2){return HOP::compare(key1, (HT)key2);}
		};
		HCompare mCompare;
	public:
		typedef HHashiter<HT, HOP, T> IterType;
		inline HHashmap() : Hashtable(&mCompare) {}
		inline ~HHashmap() {}

		inline void set(HT key, T value){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, (void *)key);
			if (pair && ! value){
				HOP::destroy(pair->key);
				delete ((T *)pair->value);
				removePair(pair);
			}else if (! pair && value){
				addPair(hash, HOP::copy(key), new T(value));
				return;
			}else if (pair){
				*((T *)pair->value) = value;
			}
		}
		inline T get(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, (void *)key);
			if (pair){
				return *((T *)pair->value);
			}
			return T();
		}
		inline grab(IterType) iterator(){
			grab(IterType) object = new IterType(Hashtable::iterator());
			return object;
		}
	};

	class CLS_EXPORT HOPString
	{
	public:
		static inline bool compare(void *savekey, const char *outerkey){
			const char *pkey = (const char *)savekey;
			return 0 == strcmp(pkey, outerkey);
		}
		static inline u32 hashKey(const char * key){
			return Utils::hashStr(key);
		}
		static inline void *copy(const char * key){
			return (void *)strdup(key);
		}
		static inline void destroy(void *savekey){
			free (savekey);
		}
		static inline const char *convert(void *savekey){
			return (const char *)savekey;
		}
	};

	class CLS_EXPORT HOPInteger
	{
	public:
		static inline bool compare(void *savekey, int outerkey){
			return (int)savekey == (int)outerkey;
		}
		static inline u32 hashKey(int key){
			return (u32)key;
		}
		static inline void *copy(int key){
			return (void *)key;
		}
		static inline void destroy(void *savekey){
		}
		static inline int convert(void *savekey){
			return (int) savekey;
		}
	};

#define strMap(CLS)			HHashmap<const char *, HOPString, CLS>
#define intMap(CLS)			HHashmap<int, HOPInteger, CLS>

}

#endif

