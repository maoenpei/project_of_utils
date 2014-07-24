#ifndef __NARAN_COMMON_DEF_H__
#define __NARAN_COMMON_DEF_H__

/* common cpp util headers */
#include <assert.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <set>
#include <map>

/* namespace def */
#define NS_NARAN			Naran
#define NS_DEF_NARAN		namespace NS_NARAN
#define NS_USING_NARAN		using namespace NS_NARAN

/* platform */
#ifdef WIN32
#define CLS_EXPORT		__declspec(dllexport)
#pragma warning(disable:4996)
#else
#define CLS_EXPORT		
#endif

/* type def */
NS_DEF_NARAN{
	typedef unsigned char u_char;
	typedef unsigned char * common_ptr;
	typedef unsigned int u32;
	typedef int i32;
	typedef unsigned char byte;
	typedef const char * c_str;
	typedef void (* func_ptr)(void);
}

/* bit operation */
#define BIT_GET(i, st, l)			(((i)>>(st)) & (~((~0)<<(l))))
#define BIT_GETB(i, n)				BIT_GET(i, (n)*8, 8)
#define BIT_PUSH(i, j, l)			(((i)<<(l)) & BIT_GET(j, 0, l))
#define BIT_PUSHB(i, j)				BIT_PUSH(i, j, 8)
#define BIT_PUSHB2(i, j, k)			BIT_PUSHB(BIT_PUSHB(i, j), k)
#define BIT_PUSHB3(i, j, k, o)		BIT_PUSHB(BIT_PUSHB2(i, j, k), o)

/* bits operation*/
#define BITS_GEN_ZEROS(L)				((~0)<<(L))
#define BITS_GEN_ONES(L)				(~BITS_GEN_ZEROS(L))

#define BITS_CHECK_ONES(VAL, N, L)		((((VAL)>>(N)) | BITS_GEN_ZEROS(L)) == ~0)
#define BITS_CHECK_ZEROS(VAL, N, L)		((((VAL)>>(N)) & BITS_GEN_ONES(L)) == 0)

#define BITS_FIRST_BITV1(VAL, N)		(BITS_CHECK_ONES(VAL, N, 1) ? (N) : (N)-1)
#define BITS_FIRST_BITV2(VAL, N)		(BITS_CHECK_ZEROS(VAL, (N)+1, 1) ? BITS_FIRST_BITV1(VAL, N) : (N)+1)
#define BITS_FIRST_BITV4(VAL, N)		(BITS_CHECK_ZEROS(VAL, (N)+2, 2) ? BITS_FIRST_BITV2(VAL, N) : BITS_FIRST_BITV2(VAL, (N)+2))
#define BITS_FIRST_BITV8(VAL, N)		(BITS_CHECK_ZEROS(VAL, (N)+4, 4) ? BITS_FIRST_BITV4(VAL, N) : BITS_FIRST_BITV4(VAL, (N)+4))
#define BITS_FIRST_BITV16(VAL, N)		(BITS_CHECK_ZEROS(VAL, (N)+8, 8) ? BITS_FIRST_BITV8(VAL, N) : BITS_FIRST_BITV8(VAL, (N)+8))
#define BITS_FIRST_BITV32(VAL, N)		(BITS_CHECK_ZEROS(VAL, (N)+16, 16) ? BITS_FIRST_BITV16(VAL, N) : BITS_FIRST_BITV16(VAL, (N)+16))
#define BITS_FIRST_BITV(VAL)			BITS_FIRST_BITV32(VAL, 0)

#define BITS_FIRST_BIT1(VAL, N)			(BITS_CHECK_ONES(VAL, N, 1) ? (N) : (N)+1)
#define BITS_FIRST_BIT2(VAL, N)			(BITS_CHECK_ZEROS(VAL, (N), 1) ? BITS_FIRST_BIT1(VAL, (N)+1) : (N))
#define BITS_FIRST_BIT4(VAL, N)			(BITS_CHECK_ZEROS(VAL, (N), 2) ? BITS_FIRST_BIT2(VAL, (N)+2) : BITS_FIRST_BIT2(VAL, N))
#define BITS_FIRST_BIT8(VAL, N)			(BITS_CHECK_ZEROS(VAL, (N), 4) ? BITS_FIRST_BIT4(VAL, (N)+4) : BITS_FIRST_BIT4(VAL, N))
#define BITS_FIRST_BIT16(VAL, N)		(BITS_CHECK_ZEROS(VAL, (N), 8) ? BITS_FIRST_BIT8(VAL, (N)+8) : BITS_FIRST_BIT8(VAL, N))
#define BITS_FIRST_BIT32(VAL, N)		(BITS_CHECK_ZEROS(VAL, (N), 16) ? BITS_FIRST_BIT16(VAL, (N)+16) : BITS_FIRST_BIT16(VAL, N))
#define BITS_FIRST_BIT(VAL)				BITS_FIRST_BIT32(VAL, 0)

#define BITS_GET(VAL, N)				(((VAL) >> (N)) & 0x1)
#define BITS_SET0(VAL, N)				((VAL) & (~(1 << (N))))
#define BITS_SET1(VAL, N)				((VAL) | (1 << (N)))

/* math func */
#define MATH_ABS(a)					((a) < 0 ? -(a) : (a))
#define MATH_MAX(a, b)				((a) > (b) ? (a) : (b))
#define MATH_MIN(a, b)				((a) < (b) ? (a) : (b))

/* common value def */
#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 1
#endif

/* utils def (may be removed to another file) */
#define CLS_STATIC(NAME, CODES)			\
	static class _STATIC_##NAME##_{public:_STATIC_##NAME##_()CODES} _VAR_##NAME##_

#ifdef WIN32
#define CLS_ASM(...)		__asm {__VA_ARGS__}
#endif

#endif

