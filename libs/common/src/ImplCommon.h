#ifndef __LIB_IMPL_COMMON_H__
#define __LIB_IMPL_COMMON_H__

#include <string>
#include <vector>
#include <list>
#include <map>
#include <stdio.h>

#define LIB_IMPL_DESTROY()		\
	virtual void destroy(){delete this;}

#define LIB_INTERFACES()		\
	virtual void destroy() = 0;

#endif

