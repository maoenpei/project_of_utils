

#include "KLongArithmetric.h"

#ifdef _NSYS_X86

#define ADD_ARGS_CARRY2(RESULT, CARRY, ARG1, ARG2)		\
	do {\
		CLS_ASM(mov eax, dword ptr[ARG1]);\
		CLS_ASM(add eax, dword ptr[ARG2]);\
		CLS_ASM(jnb label_ncarry);\
		CLS_ASM(add dword ptr[CARRY], 1);\
		CLS_ASM(label_ncarry:);\
		CLS_ASM(mov dword ptr[RESULT], eax);\
	} while(0)

#define ADD_ARGS_CARRY3(RESULT, CARRY, ARG1, ARG2, ARG3)		\
	do {\
		CLS_ASM(mov eax, dword ptr[ARG1]);\
		CLS_ASM(add eax, dword ptr[ARG2]);\
		CLS_ASM(jnb label_nlcarry);\
		CLS_ASM(add dword ptr[CARRY], 1);\
		CLS_ASM(label_nlcarry:);\
		CLS_ASM(add eax, dword ptr[ARG3]);\
		CLS_ASM(jnb label_nlcarry2);\
		CLS_ASM(add dword ptr[CARRY], 1);\
		CLS_ASM(label_nlcarry2:);\
		CLS_ASM(mov dword ptr[RESULT], eax);\
	} while(0)

#define SUB_ARGS_CARRY2(RESULT, CARRY, ARG1, ARG2)		\
	do {\
		CLS_ASM(mov eax, dword ptr[arg1]);\
		CLS_ASM(sub eax, dword ptr[arg2]);\
		CLS_ASM(jnb label_ncarry);\
		CLS_ASM(add dword ptr[CARRY], 1);\
		CLS_ASM(label_ncarry:);\
		CLS_ASM(mov dword ptr[RESULT], eax);\
	} while(0)

#define SUB_ARGS_CARRY3(RESULT, CARRY, ARG1, ARG2, ARG3)		\
	do {\
		CLS_ASM(mov eax, dword ptr[ARG1]);\
		CLS_ASM(sub eax, dword ptr[ARG2]);\
		CLS_ASM(jnb label_nlcarry);\
		CLS_ASM(add dword ptr[CARRY], 1);\
		CLS_ASM(label_nlcarry:);\
		CLS_ASM(sub eax, dword ptr[ARG3]);\
		CLS_ASM(jnb label_nlcarry2);\
		CLS_ASM(add dword ptr[CARRY], 1);\
		CLS_ASM(label_nlcarry2:)\
		CLS_ASM(mov dword ptr[RESULT], eax);\
	} while(0)

#define MUL_DEVIDED(HI, LO, ARG1, ARG2)		\
	do {\
		CLS_ASM(mov eax, dword ptr[ARG1]);\
		CLS_ASM(mul dword ptr[ARG2]);\
		CLS_ASM(mov dword ptr[LO], eax);\
		CLS_ASM(mov dword ptr[HI], edx);\
	} while(0)

#define DIV_DEVIDED(QUOT, REM, ARG1, ARG2, ARG3)	\
	do {\
		CLS_ASM(mov edx, dword ptr[ARG1]);\
		CLS_ASM(mov eax, dword ptr[ARG2]);\
		CLS_ASM(div dword ptr[ARG3]);\
		CLS_ASM(mov dword ptr[QUOT], eax);\
		CLS_ASM(mov dword ptr[REM], edx);\
	} while(0)

#endif

#define NEG(DATA)		(((DATA) [mLen-1] >> 31) != 0)

NS_DEF_NARAN{

	/* return data1 > data2; */
	static inline int rawCompare(const u32 data1[], const u32 data2[], int mLen)
	{
		for (int i = mLen - 1; i>= 0; i--){
			if (data1[i] > data2[i])
				return 1;
			if (data1[i] < data2[i])
				return -1;
		}
		return 0;
	}

	/*
	result = data << l;
	*/
	static inline void rawShiftLeft(u32 result[], const u32 data[], int l, int mLen)
	{
		bool left = (l > 0);
		l = (l < 0 ? -l : l);
		int block = l >> 5;
		int inner = l & 0x1f;
		int rinner = 32 - inner;
		if (block >= mLen){
			memset(result, 0, mLen * 4);
			return ;
		}
		if ((inner & 0x7) == 0){
			const byte *src = (const byte *)data;
			byte *dst = (byte *)result;
			int offset = block * 4 + (inner >> 3);
			if (left){
				memmove(dst + offset, src, (mLen * 4) - offset);
				memset(dst, 0, offset);
			} else {
				memmove(dst, src + offset, (mLen * 4) - offset);
				memset(dst + (mLen * 4) - offset, 0, offset);
			}
			return;
		}
		u32 arg1, arg2;
		if (left){
			for (int i = mLen-1; i>=0; i--){
				if (i-block < 0){
					result[i] = 0;
				}else{
					arg1 = data[i-block];
					arg2 = (i-block-1 >= 0 ? data[i-block-1] : 0);
					result[i] = (arg1 << inner) | (arg2 >> rinner);
				}
			}
		} else {
			for (int i = 0; i<mLen; i++){
				if (i+block > mLen){
					result[i] = 0;
				}else{
					arg1 = data[i+block];
					arg2 = (i+block+1 < mLen ? data[i+block+1] : 0);
					result[i] = (arg1 >> inner) | (arg2 << rinner);
				}
			}
		}
	}

	/*
	get first bit offset of data
	*/
	static inline int getFirstBit(const u32 data[], int mLen)
	{
		for (int i = mLen - 1; i>=0; i--){
			if (data[i]){
				register u32 arg = data[i];
				return i * 32 + BITS_FIRST_BITV(arg);
			}
		}
		return -1;
	}

	static inline int getBit(u32 data[], int l, int mLen)
	{
		int block = l >> 5;
		int inner = l & 0x1f;
		if (l < 0 || block >= mLen){
			return 0;
		}
		return BITS_GET(data[block], inner);
	}

	static inline void setBit(u32 data[], int l, int bit, int mLen)
	{
		int block = l >> 5;
		int inner = l & 0x1f;
		if (l < 0 || block >= mLen){
			return;
		}
		data[block] = (bit ? BITS_SET1(data[block], inner) : BITS_SET0(data[block], inner));
	}

	bool LongIntAlgrithm::Equal(const u32 data1[], const u32 data2[])
	{
		return 0 == memcmp(data1, data2, mLen * sizeof(u32));
	}

	void LongIntAlgrithm::AndOp(u32 result[], const u32 data1[], const u32 data2[])
	{
		for (int i = 0; i<mLen; i++){
			result[i] = data1[i] & data2[i];
		}
	}

	void LongIntAlgrithm::OrOp(u32 result[], const u32 data1[], const u32 data2[])
	{
		for (int i = 0; i<mLen; i++){
			result[i] = data1[i] | data2[i];
		}
	}

	void LongIntAlgrithm::XorOp(u32 result[], const u32 data1[], const u32 data2[])
	{
		for (int i = 0; i<mLen; i++){
			result[i] = data1[i] ^ data2[i];
		}
	}

	void LongIntAlgrithm::ComplementOp(u32 result[], const u32 data[])
	{
		for (int i = 0; i<mLen; i++){
			result[i] = ~ data[i];
		}
	}

	/*
	shift left l bits
	*/
	void LongIntAlgrithm::ShiftLeft(u32 result[], const u32 data[], int l)
	{
		rawShiftLeft(result, data, l, mLen);
	}

	/*
	shift right r bits
	*/
	void LongIntAlgrithm::ShiftRight(u32 result[], const u32 data[], int r)
	{
		rawShiftLeft(result, data, -r, mLen);
	}

	int LongIntAlgrithm::Compare(const u32 data1[], const u32 data2[])
	{
		bool neg1 = NEG(data1);
		bool neg2 = NEG(data2);
		if (! neg1 && neg2)
			return 1;
		if (neg1 && ! neg2)
			return -1;
		return rawCompare(data1, data2, mLen);
	}

	/*
	result[i] = ~data[i];
	result[0] += 1;
	*/
	void LongIntAlgrithm::Negative(u32 result[], const u32 data[])
	{
		u32 arg1;
		u32 carry, last_carry = 1;
		for (int i = 0; i<mLen; i++){
			arg1 = ~data[i];
			
			carry = 0;
			ADD_ARGS_CARRY2(arg1, carry, arg1, last_carry);
			last_carry = carry;
			
			result[i] = arg1;
		}
	}

	/*
	result[i] = data1[i] + data2[i];
	*/
	void LongIntAlgrithm::Plus(u32 result[], const u32 data1[], const u32 data2[])
	{
		u32 arg1, arg2;
		u32 carry, carry_last = 0;
		for (int i = 0; i<mLen; i++){
			arg1 = data1[i];
			arg2 = data2[i];
			
			carry = 0;
			ADD_ARGS_CARRY3(arg1, carry, arg1, arg2, carry_last);
			carry_last = carry;
			
			result[i] = arg1;
		}
	}

	/*
	result[i] = data1[i] - data2[i];
	*/
	void LongIntAlgrithm::Minus(u32 result[], const u32 data1[], const u32 data2[])
	{
		u32 arg1, arg2;
		u32 carry, carry_last = 0;
		for (int i = 0; i<mLen; i++){
			arg1 = data1[i];
			arg2 = data2[i];

			carry = 0;
			SUB_ARGS_CARRY3(arg1, carry, arg1, arg2, carry_last);
			carry_last = carry;

			result[i] = arg1;
		}
	}

	/*
	result[i+j] += data1[i] * data2[j];
	result[i+j+1] += hi-dword(data1[i] * data2[j]);
	*/
	void LongIntAlgrithm::Multiply(u32 result[], const u32 data1[], const u32 data2[])
	{
		u32 arg1, arg2;
		u32 rlo, rhi;
		for (int i = 0; i<mLen; i++){
			u32 carry, carry_last = 0;
			for (int j = 0; j<(mLen-i); j++){
				carry = 0;
				arg1 = data1[i];
				arg2 = data2[j];
				
				MUL_DEVIDED(arg2, arg1, arg1, arg2);
				
				rlo = result[i+j];
				
				ADD_ARGS_CARRY2(arg1, carry_last, arg1, rlo);
				
				result[i+j] = arg1;
				
				if (i+j+1 < mLen){
					rhi = result[i+j+1];
					
					carry = 0;
					ADD_ARGS_CARRY3(arg2, carry, arg2, rhi, carry_last);
					carry_last = carry;
					
					result[i+j+1] = arg2;
				}
			}
		}
	}

	/*
	complex algrithm
	*/
	void LongIntAlgrithm::Divide(u32 result1[], u32 result2[], const u32 data1[], const u32 data2[])
	{
		bool allSame = true;
		for (int i = 1; i<mLen; i++){
			if (data2[i] != 0 && ~data2[i] != 0){
				allSame = false;
				break;
			}
		}
		if (allSame){
			int sresult2;
			Divide32(result1, sresult2, data1, (int)data2[0]);
			result2[0] = (u32)sresult2;
			memset(result2 + 1, (sresult2 < 0 ? 0xff : 0), sizeof(u32) * (mLen - 1));
			return;
		}
		// ready devider & devisor
		u32 *deviderData = NULL;
		const u32 *devider = (NEG(data1) ? (deviderData = new u32[mLen]()) : data1);
		u32 *devisorData = NULL;
		const u32 *divisor = (NEG(data2) ? (devisorData = new u32[mLen]()) : data2);
		if (deviderData)
			Negative(deviderData, data1);
		if (devisorData)
			Negative(devisorData, data2);
		
		// main loop
		u32 *exchange = new u32[mLen]();
		memcpy(result2, devider, mLen * sizeof(u32));
		int fdivisor = getFirstBit(divisor, mLen);
		// devide 0
		if (fdivisor >= 0){
			while(rawCompare(result2, divisor, mLen) >= 0){
				int fresult2 = getFirstBit(result2, mLen);
				// already zero
				if (fresult2 < 0)
					break;
				int fdiff = fresult2 - fdivisor;
				rawShiftLeft(exchange, divisor, fdiff, mLen);
				if (rawCompare(result2, exchange, mLen) >= 0){
					Minus(result2, result2, exchange);
					setBit(result1, fdiff, 1, mLen);
				}else{
					fdiff--;
					rawShiftLeft(exchange, divisor, fdiff, mLen);
					Minus(result2, result2, exchange);
					setBit(result1, fdiff, 1, mLen);
				}
			}
		}

		// ready result
		if (! deviderData != ! devisorData){
			Negative(result1, result1);
		}
		if (deviderData){
			Negative(result2, result2);
		}

		// clean up
		delete [] exchange;
		if (deviderData)
			delete [] deviderData;
		if (devisorData)
			delete [] devisorData;
	}

	void LongIntAlgrithm::Multiply32(u32 result[], const u32 data1[], int data2)
	{
		u32 arg1, arg2 = (u32)(data2 < 0 ? -data2 : data2);
		u32 carry = 0;
		u32 hi, lo;

		u32 *deviderData = NULL;
		const u32 *devider = (NEG(data1) ? (deviderData = new u32[mLen]()) : data1);
		if (deviderData)
			Negative(deviderData, data1);

		for (int i = 0; i<mLen; i++){
			arg1 = devider[i];
			MUL_DEVIDED(hi, lo, arg1, arg2);

			ADD_ARGS_CARRY2(arg1, hi, lo, carry);
			result[i] = arg1;
			carry = hi;
		}

		if ((deviderData == NULL && data2 < 0) || (deviderData && data2 > 0)){
			Negative(result, result);
		}

		if (deviderData)
			delete [] deviderData;
	}

	void LongIntAlgrithm::Divide32(u32 result[], int &result2, const u32 data1[], int data2)
	{
		if (data2 == 0){
			return;
		}
		u32 arg1, arg2 = 0, arg3 = (u32)(data2 < 0 ? -data2 : data2);

		u32 *deviderData = NULL;
		const u32 *devider = (NEG(data1) ? (deviderData = new u32[mLen]()) : data1);
		if (deviderData)
			Negative(deviderData, data1);

		for (int i = 0; i<mLen; i++){
			arg1 = devider[i];
			DIV_DEVIDED(arg1, arg2, arg2, arg1, arg3);
			result[i] = arg1;
		}

		if ((deviderData == NULL && data2 < 0) || (deviderData && data2 > 0)){
			Negative(result, result);
		}
		result2 = (int)arg2;
		result2 = (data2 < 0 ? -result2 : result2);

		if (deviderData)
			delete [] deviderData;
	}

}

