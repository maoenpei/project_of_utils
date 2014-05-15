#ifndef __NARAN_LONG_INT_H__
#define __NARAN_LONG_INT_H__

#include "core/CommonDef.h"

NS_DEF_NARAN{

	class LongIntAlgrithm
	{
		int mLen;
		int rawCompare(const u32 data1[], const u32 data2[]);
		int getFirstBit(const u32 data[]);
		void rawShiftLeft(u32 result[], const u32 data[], int l);
		int getBit(u32 data[], int l);
		void setBit(u32 data[], int l, int bit);
	public:
		LongIntAlgrithm(int len);
		void AndOp(u32 result[], const u32 data1[], const u32 data2[]);
		void OrOp(u32 result[], const u32 data1[], const u32 data2[]);
		void XorOp(u32 result[], const u32 data1[], const u32 data2[]);
		void ComplementOp(u32 result[], const u32 data[]);
		void ShiftLeft(u32 result[], const u32 data[], int l);
		void ShiftRight(u32 result[], const u32 data[], int r);
		bool Equal(const u32 data1[], const u32 data2[]);
		int Compare(const u32 data1[], const u32 data2[]);
		void Negative(u32 result[], const u32 data[]);
		void Plus(u32 result[], const u32 data1[], const u32 data2[]);
		void Minus(u32 result[], const u32 data1[], const u32 data2[]);
		void Multiply(u32 result[], const u32 data1[], const u32 data2[]);
		void Divide(u32 result1[], u32 result2[], const u32 data1[], const u32 data2[]);
	};

	template<int LEN>
	class LongInt
	{
		u32 data[LEN];
	public:
		inline LongInt(){
			memset(data, 0, sizeof(data));
		}
		inline LongInt(int ni){
			memset(data, (ni < 0 ? 0xff : 0), sizeof(data));
			data[0] = (u32)ni;
		}
		inline LongInt(const LongInt &copy){
			memcpy(data, copy.data, sizeof(data));
		}
		inline LongInt(const u32 *_data){
			memcpy(data, _data, sizeof(data));
		}
		inline LongInt(int count, u32 min, ...){
			va_list vl;
			va_start(vl, count);
			for (int i = 0; i<count && i<LEN; i++){
				data[i] = va_arg(vl, u32);
			}
			va_end(vl);
		}
		inline int get(){
			return (int)data[0];
		}
		inline void get(u32 _data[]){
			memcpy(_data, data, sizeof(data));
		}
		inline void getDivision(const LongInt &other, LongInt &quotient, LongInt &remainder)
		{
			LongIntAlgrithm(LEN).Divide(quotient.data, remainder.data, data, other.data);
		}

		inline LongInt &operator =(const LongInt &other){
			memcpy(data, other.data, sizeof(data));
			return *this;
		}
		inline LongInt operator &(const LongInt &other) const{
			LongInt li;
			LongIntAlgrithm(LEN).AndOp(li.data, data, other.data);
			return li;
		}
		inline LongInt operator |(const LongInt &other) const{
			LongInt li;
			LongIntAlgrithm(LEN).OrOp(li.data, data, other.data);
			return li;
		}
		inline LongInt operator ^(const LongInt &other) const{
			LongInt li;
			LongIntAlgrithm(LEN).XorOp(li.data, data, other.data);
			return li;
		}
		inline LongInt operator ~() const{
			LongInt li;
			LongIntAlgrithm(LEN).ComplementOp(li.data, data);
			return li;
		}
		inline operator bool() const{
			LongInt li;
			return ! LongIntAlgrithm(LEN).Equal(data, li.data);
		}
		inline bool operator == (const LongInt &other) const{
			return LongIntAlgrithm(LEN).Equal(data, other.data);
		}
		inline bool operator != (const LongInt &other) const{
			return ! LongIntAlgrithm(LEN).Equal(data, other.data);
		}
		inline bool operator > (const LongInt &other) const{
			return LongIntAlgrithm(LEN).Compare(data, other.data) > 0;
		}
		inline bool operator < (const LongInt &other) const{
			return LongIntAlgrithm(LEN).Compare(data, other.data) < 0;
		}
		inline bool operator >= (const LongInt &other) const {
			return LongIntAlgrithm(LEN).Compare(data, other.data) >= 0;
		}
		inline bool operator <= (const LongInt &other) const {
			return LongIntAlgrithm(LEN).Compare(data, other.data) <= 0;
		}
		inline LongInt operator <<(int bits) const {
			LongInt li;
			LongIntAlgrithm(LEN).ShiftLeft(li.data, data, bits);
			return li;
		}
		inline LongInt operator >>(int bits) const{
			LongInt li;
			LongIntAlgrithm(LEN).ShiftRight(li.data, data, bits);
			return li;
		}
		inline LongInt operator -() const{
			LongInt li;
			LongIntAlgrithm(LEN).Negative(li.data, data);
			return li;
		}
		inline LongInt operator +(const LongInt &other) const {
			LongInt li;
			LongIntAlgrithm(LEN).Plus(li.data, data, other.data);
			return li;
		}
		inline LongInt &operator ++() const{
			LongInt li(1);
			LongIntAlgrithm(LEN).Plus(data, data, li.data);
			return *this;
		}
		inline LongInt operator -(const LongInt &other) const{
			LongInt li;
			LongIntAlgrithm(LEN).Minus(li.data, data, other.data);
			return li;
		}
		inline LongInt &operator --() const{
			LongInt li(1);
			LongIntAlgrithm(LEN).Minus(data, data, li.data);
			return *this;
		}
		inline LongInt operator *(const LongInt &other) const {
			LongInt li;
			LongIntAlgrithm(LEN).Multiply(li.data, data, other.data);
			return li;
		}
		inline LongInt operator /(const LongInt &other) const{
			LongInt quotient, remainder;
			LongIntAlgrithm(LEN).Divide(quotient.data, remainder.data, data, other.data);
			return quotient;
		}
		inline LongInt operator %(const LongInt &other) const{
			LongInt quotient, remainder;
			LongIntAlgrithm(LEN).Divide(quotient.data, remainder.data, data, other.data);
			return remainder;
		}
	};

	typedef LongInt<2>		LLint;
	typedef LongInt<8>		ExLong;

}

#endif

