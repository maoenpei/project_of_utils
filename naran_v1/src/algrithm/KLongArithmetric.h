#ifndef __NARAN_LONG_INT_H__
#define __NARAN_LONG_INT_H__

#include "CommonDef.h"

NS_DEF_NARAN{

	class CLS_EXPORT LongIntAlgrithm
	{
		int mLen;
	public:
		inline LongIntAlgrithm(int len) : mLen(len) {}
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
		void Multiply32(u32 result[], const u32 data1[], int data2);
		void Divide32(u32 result[], int &result2, const u32 data1[], int data2);
	};

}

#endif

