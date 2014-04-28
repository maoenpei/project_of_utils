#ifndef __LIB_BUILD_COMMON_H__
#define __LIB_BUILD_COMMON_H__

#ifdef WIN32
#pragma warning(disable:4996)
#endif

#ifdef WIN32
#define EXPORT_FUNC		__declspec(dllexport)
#else
#define EXPORT_FUNC		
#endif

#endif

