#ifndef __NARAN_LONG_LONG_H__
#define __NARAN_LONG_LONG_H__

#include "CommonDef.h"
#include "algrithm/KLongArithmetric.h"

NS_DEF_NARAN{

	template<int LEN>
	class CLS_EXPORT LongInt
	{
		u32 data[LEN];
	public:
		inline LongInt(){
			memset(data, 0, sizeof(data));
		}
		explicit inline LongInt(int ni){
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
		inline LongInt operator *(int mul) const{
			LongInt li;
			LongIntAlgrithm(LEN).Multiply32(li.data, data, mul);
			return li;
		}
		inline LongInt operator /(const LongInt &divisor) const{
			LongInt quotient, remainder;
			LongIntAlgrithm(LEN).Divide(quotient.data, remainder.data, data, divisor.data);
			return quotient;
		}
		inline LongInt operator %(const LongInt &divisor) const{
			LongInt quotient, remainder;
			LongIntAlgrithm(LEN).Divide(quotient.data, remainder.data, data, divisor.data);
			return remainder;
		}
		inline void devide(const LongInt &divisor, LongInt &quot, LongInt &rem) const{
			LongIntAlgrithm(LEN).Divide(quot.data, rem.data, data, divisor.data);
		}
		inline LongInt operator /(int divisor) const{
			LongInt quot;
			int rem;
			LongIntAlgrithm(LEN).Divide32(quot.data, rem, data, divisor);
			return quot;
		}
		inline int operator %(int divisor) const{
			LongInt quot;
			int rem;
			LongIntAlgrithm(LEN).Divide32(quot.data, rem, data, divisor);
			return rem;
		}
	};

	typedef LongInt<2>		LLint;
	typedef LongInt<8>		ExLong;
#define longint(bits)		LongInt<bits/32>

}

#endif

