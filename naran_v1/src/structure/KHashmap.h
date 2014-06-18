/***************************************

HHashmap/HHashset uses hash table but provide more friendly interface.

Key type must be simple and takes just one dword size.
Value type should also be simple and takes one or two dword size.
HOP indicates operation of key type.

***************************************/
#ifndef __NARAN_HASH_MAP_H__
#define __NARAN_HASH_MAP_H__

#include "CommonDef.h"
#include "framework/Utils.h"
#include "algrithm/KHashtable.h"
#include "autoref/AutoReference.h"

NS_DEF_NARAN{
	
	/* iterator of hashmap */
	template<class HT, class HOP, class T>
	class CLS_EXPORT HHashmapIter
	{
		grab(Hashiter) mIter;
		HHashmapIter(Hashiter * iter) : mIter(iter) {
			next();
		}
		template<class _HT, class _HOP, class _T>
		friend class HHashmap;
	public:
		HT key;
		T value;
		bool hasNext;
		inline void next(){
			HashPair *pair = mIter->next();
			hasNext = pair != NULL;
			if (pair){
				key = (HT)(pair->key);
				value = *((T *)pair->value);
			}
		}
	};

	/* hashmap */
	template<class HT, class HOP, class T>
	class CLS_EXPORT HHashmap : public Hashtable
	{
		class CLS_EXPORT HCompare : public IHashCompare
		{
		public:
			virtual bool compare(void *key1, void *key2){return HOP::compare(key1, key2);}
		};
		HCompare mCompare;
		HHashmap(const HHashmap &copy){}
	public:
		typedef HHashmapIter<HT, HOP, T> IterType;
		inline HHashmap() : Hashtable(&mCompare) {}
		inline ~HHashmap() {}

		inline void set(HT key, const T &value){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, (void *)key);
			if (! pair){
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
		inline void remove(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, (void *)key);
			if (pair){
				delete ((T *)pair->value);
				HOP::destroy(pair->key);
				removePair(pair);
			}
		}
		inline IterType * iterator(){
			return new IterType(Hashtable::iterator());
		}
	};
	
	/* iterator of hashset */
	template<class HT, class HOP>
	class CLS_EXPORT HHashsetIter
	{
		grab(Hashiter) mIter;
		HHashsetIter(Hashiter * iter) : mIter(iter) {
			next();
		}
		template<class _HT, class _HOP>
		friend class HHashset;
	public:
		HT value;
		bool hasNext;
		inline void next(){
			HashPair *pair = mIter->next();
			hasNext = pair != NULL;
			if (pair){
				value = (HT)(pair->key);
			}
		}
	};

	/* hashset */
	template<class HT, class HOP>
	class HHashset : public Hashtable
	{
		class CLS_EXPORT HCompare : public IHashCompare
		{
		public:
			virtual bool compare(void *key1, void *key2){return HOP::compare(key1, key2);}
		};
		HCompare mCompare;
		HHashset(const HHashset &copy){}
	public:
		typedef HHashsetIter<HT, HOP> IterType;
		inline HHashset() : Hashtable(&mCompare) {}
		inline ~HHashset() {}

		inline void append(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, (void *)key);
			if (! pair){
				addPair(hash, HOP::copy(key), NULL);
				return;
			}
		}
		inline bool has(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, (void *)key);
			return pair != NULL;
		}
		inline void remove(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, (void *)key);
			if (pair){
				HOP::destroy(pair->key);
				removePair(pair);
			}
		}
		inline IterType * iterator(){
			return new IterType(Hashtable::iterator());
		}
		
	};

	/* HOP of string */
	typedef const char * HTString;
	class CLS_EXPORT HOPString
	{
	public:
		static inline bool compare(void *savekey, void *outerkey){
			HTString pkey = (HTString)savekey;
			return 0 == strcmp(pkey, (HTString)outerkey);
		}
		static inline u32 hashKey(HTString key){
			return Utils::hashStr(key);
		}
		static inline void *copy(HTString key){
			return (void *)strdup(key);
		}
		static inline void destroy(void *savekey){
			free (savekey);
		}
	};

	/* HOP of 4-bytes value */
	template<class T>
	class CLS_EXPORT HOPCommon
	{
	public:
		static inline bool compare(void *savekey, void * outerkey){
			return savekey == outerkey;
		}
		static inline u32 hashKey(T key){
			return (u32)key;
		}
		static inline void *copy(T key){
			return (void *)key;
		}
		static inline void destroy(void *savekey){
		}
	};

#define strMap(CLS)				HHashmap<HTString, HOPString, CLS>
#define intMap(CLS)				HHashmap<int, HOPCommon<int>, CLS>
#define objectMap(KEY, VAL)		HHashmap<KEY *, HOPCommon<KEY *>, VAL>

#define strSet()				HHashset<HTString, HOPString>
#define varSet(CLS)				HHashset<CLS, HOPCommon<CLS>>

}

#endif

