#ifndef __NARAN_COMMON_DEF_H__
#define __NARAN_COMMON_DEF_H__

/* common cpp util headers */
#include <assert.h>
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

/* bit opration */
#define BIT_GET(i, st, l)			(((i)>>(st)) & (~((~0)<<(l))))
#define BIT_GETB(i, n)				BIT_GET(i, (n)*8, 8)
#define BIT_PUSH(i, j, l)			(((i)<<(l)) & BIT_GET(j, 0, l))
#define BIT_PUSHB(i, j)				BIT_PUSH(i, j, 8)
#define BIT_PUSHB2(i, j, k)			BIT_PUSHB(BIT_PUSHB(i, j), k)
#define BIT_PUSHB3(i, j, k, o)		BIT_PUSHB(BIT_PUSHB2(i, j, k), o)

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

#endif

