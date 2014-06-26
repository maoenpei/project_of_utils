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
				key = HOP::revert(pair->key);
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

		inline bool hasKey(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, HOP::convert(key));
			return pair != NULL;
		}
		inline void set(HT key, const T &value){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, HOP::convert(key));
			if (! pair){
				addPair(hash, HOP::copy(key), new T(value));
				return;
			}else if (pair){
				*((T *)pair->value) = value;
			}
		}
		inline T get(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, HOP::convert(key));
			if (pair){
				return *((T *)pair->value);
			}
			return T();
		}
		inline void remove(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, HOP::convert(key));
			if (pair){
				delete ((T *)pair->value);
				HOP::destroy(pair->key);
				removePair(pair);
			}
		}
		inline IterType iterator(){
			return IterType(Hashtable::iterator());
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
				value = HOP::revert(pair->key);
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

		inline void insert(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, HOP::convert(key));
			if (! pair){
				addPair(hash, HOP::copy(key), NULL);
				return;
			}
		}
		inline bool has(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, HOP::convert(key));
			return pair != NULL;
		}
		inline void remove(HT key){
			u32 hash = HOP::hashKey(key);
			HashPair *pair = findPair(hash, HOP::convert(key));
			if (pair){
				HOP::destroy(pair->key);
				removePair(pair);
			}
		}
		inline IterType iterator(){
			return IterType(Hashtable::iterator());
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
		static inline void *convert(HTString key){
			return (void *)key;
		}
		static inline HTString revert(void *savekey){
			return (HTString)savekey;
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
			assert(sizeof(T) == sizeof(int));
			return (u32)key;
		}
		static inline void *copy(T key){
			return (void *)key;
		}
		static inline void destroy(void *savekey){
		}
		static inline void *convert(T key){
			assert(sizeof(T) == sizeof(int));
			return (void *)key;
		}
		static inline T revert(void *savekey){
			return (T)savekey;
		}
	};

	/* HOP of packaged class with method get */
	template<class T>
	class CLS_EXPORT HOPPackaged
	{
	public:
		static inline bool compare(void *savekey, void * outerkey){
			return (*(T *)savekey).get() == (*(T *)outerkey).get();
		}
		static inline u32 hashKey(const T &key){
			return (u32)key.get();
		}
		static inline void *copy(const T &key){
			return (void *)new T(key);
		}
		static inline void destroy(void *savekey){
			delete ((T *)savekey);
		}
		static inline void *convert(const T &key){
			return (void *)&key;
		}
		static inline T revert(void *savekey){
			return *(T *)savekey;
		}
	};

#define strMap(CLS)				HHashmap<HTString, HOPString, CLS>
#define intMap(CLS)				HHashmap<int, HOPCommon<int>, CLS>
#define varMap(TYPE, CLS)		HHashmap<TYPE, HOPCommon<TYPE>, CLS>
#define grabMap(KEY, VAL)		HHashmap<grab(KEY), HOPPackaged<grab(KEY)>, VAL>
#define interfMap(KEY, VAL)		HHashmap<interf(KEY), HOPPackaged<interf(KEY)>, VAL>

#define strSet()				HHashset<HTString, HOPString>
#define varSet(TYPE)			HHashset<TYPE, HOPCommon<TYPE>>
#define grabSet(CLS)			HHashset<grab(CLS), HOPPackaged<grab(CLS)>>
#define interfSet(CLS)			HHashset<interf(CLS), HOPPackaged<interf(CLS)>>

}

#endif

