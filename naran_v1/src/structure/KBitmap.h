#ifndef __NARAN_BITMAP_H__
#define __NARAN_BITMAP_H__

#include "CommonDef.h"
#include "allocator/CAllocator.h"

NS_DEF_NARAN{

#define BITS_COUNT		(32)
#define BITS_TYPE		u32

	class CLS_EXPORT Bitmap
	{
	protected:
		int mDwords;
		int mBits;
		BITS_TYPE *mPtr;
	public:
		inline Bitmap() : mPtr(NULL), mBits(0), mDwords(0) {}
		inline Bitmap(int bits) : mBits(bits) {
			assert(bits > 0);
			mDwords = ((mBits - 1) / BITS_COUNT) + 1;
			mPtr = (BITS_TYPE *)Allocator::allocArrayShare(sizeof(BITS_TYPE) * mDwords);
			memset(mPtr, 0, sizeof(BITS_TYPE) * mDwords);
		}
		inline ~Bitmap(){
			if (mPtr){
				Allocator::freeArrayShare(mPtr);
			}
		}
		inline Bitmap(const Bitmap &copy) : mPtr(NULL) {
			mBits = copy.mBits;
			mDwords = copy.mDwords;
			if (copy.mPtr){
				mPtr = (BITS_TYPE *)Allocator::allocArrayShare(sizeof(BITS_TYPE) * mDwords);
				memcpy(mPtr, copy.mPtr, sizeof(BITS_TYPE) * mDwords);
			}
		}
		
		inline int count() const{
			return mBits;
		}
		inline void setAllBits(bool isOne){
			if (mPtr){
				memset(mPtr, (isOne ? 0xff : 0), sizeof(BITS_TYPE) * mDwords);
			}
		}
		inline void setBits(bool isOne, int offset){
			assert(offset >= 0 && offset < mBits);
			if (mPtr){
				int od = offset / BITS_COUNT, ob = offset % BITS_COUNT;
				if (isOne){
					mPtr[od] |= (1 << ob);
				}else{
					mPtr[od] &= ~(1 << ob);
				}
			}
		}
		inline bool getBits(int offset) const{
			assert(offset >= 0 && offset < mBits);
			if (mPtr){
				int od = offset / BITS_COUNT, ob = offset % BITS_COUNT;
				return (mPtr[od] & (1 << ob)) != 0;
			}
			return false;
		}
		inline int getFirstBit(bool isOne) const{
			if (mPtr){
				BITS_TYPE u;
				for (int i = 0; i<mDwords; i++){
					if (isOne && mPtr[i]){
						u = mPtr[i];
						u = BITS_FIRST_BIT(u);
						return i * BITS_COUNT + u;
					}else if (!isOne && ~mPtr[i]){
						u = ~mPtr[i];
						u = BITS_FIRST_BIT(u);
						return i * BITS_COUNT + u;
					}
				}
			}
			return -1;
		}
	};

	class CLS_EXPORT Blocks : public Bitmap
	{
		int mBlockSize;
		byte *mBlocks;
	public:
		inline Blocks() : Bitmap(), mBlockSize(0), mBlocks(NULL){}
		inline Blocks(int blocks, int block_size) : Bitmap(), mBlockSize(block_size){
			assert(blocks > 0);
			mBits = blocks;
			mDwords = ((mBits - 1) / BITS_COUNT) + 1;
			mPtr = (BITS_TYPE *)Allocator::allocArrayShare(sizeof(BITS_TYPE) * mDwords + mBlockSize * mBits);
			memset(mPtr, 0, sizeof(BITS_TYPE) * mDwords + mBlockSize * mBits);
			mBlocks = (byte *)(mPtr + mDwords);
		}
		inline Blocks(const Blocks &copy){
			mBlockSize = copy.mBlockSize;
			mBits = copy.mBits;
			mDwords = copy.mDwords;
			if (copy.mPtr){
				mPtr = (BITS_TYPE *)Allocator::allocArrayShare(sizeof(BITS_TYPE) * mDwords + mBlockSize * mBits);
				memset(mPtr, 0, sizeof(BITS_TYPE) * mDwords + mBlockSize * mBits);
				mBlocks = (byte *)(mPtr + mDwords);
			}else{
				mPtr = NULL;
				mBlocks = NULL;
			}
		}
		
		inline byte *getBlock(int index){
			assert(index >= 0 && index < mBits);
			return mBlocks + mBlockSize * index;
		}
		inline int getIndex(byte *block){
			assert(block >= mBlocks && block < mBlocks + mBlockSize * mBits && (block - mBlocks) % mBlockSize == 0);
			return (block - mBlocks) / mBlockSize;
		}
		inline void copyFromOther(const Blocks &copy){
			assert(mBlockSize == copy.mBlockSize);
			int blocks = (mBits < copy.mBits ? mBits : copy.mBits);
			if (blocks > 0 && mPtr && copy.mPtr){
				int od = blocks / BITS_COUNT, ob = blocks % BITS_COUNT;
				memcpy(mPtr, copy.mPtr, sizeof(BITS_TYPE) * od);
				if (ob > 0){
					mPtr[od] = (mPtr[od] & ((~0) << ob)) | (copy.mPtr[od] & (~((~0) << ob)));
				}
				memcpy(mBlocks, copy.mBlocks, mBlockSize * blocks);
			}
		}
	};

}

#endif

