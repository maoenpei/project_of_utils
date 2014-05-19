#ifndef __NARAN_LINKED_LIST_H__
#define __NARAN_LINKED_LIST_H__

#include "core/CommonDef.h"
#include "core/Utils.h"

NS_DEF_NARAN{

	template<typename T>
	class CLS_EXPORT LinkedNode
	{
		T mValue;
		grab(LinkedNode) mNext;
	public:
		void set(T v){
			mValue = v;
		}
		T get(){
			return mValue;
		}
		grab(LinkedNode) &next(){
			return mNext;
		}
	};

	template<typename T>
	class CLS_EXPORT LinkedList
	{
		grab(LinkedNode<T>) mHead;
	public:
		grab(LinkedNode<T>) &getHead() {return mHead;}
		int count() {
			grab(LinkedNode<T>) tmp = mHead;
			int c = 0;
			while(tmp){
				c++;
				tmp = mHead->next();
			}
			return c;
		}
	};

}

#endif

