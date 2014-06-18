/***************************************

Hashtable provide algrithm of hash table.

***************************************/
#ifndef __NARAN_HASH_TABLE_H__
#define __NARAN_HASH_TABLE_H__

#include "CommonDef.h"

NS_DEF_NARAN{

	struct HashKey
	{
		u32 uHash;
		void *key;
	};

	/* compare method */
	class IHashCompare
	{
	public:
		// compare inner key to outer key
		virtual bool compare(void *key1, void *key2) = 0;
	};

	struct HashPair : public HashKey
	{
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
		Hashtable(IHashCompare *compare);
		~Hashtable();

		HashPair * findPair(u32 hash, void *key);
		void addPair(u32 hash, void *key, void *value);
		void removePair(HashKey *pair);
		Hashiter * iterator();

	private:
		void *mHashData;
	};
	
}

#endif

