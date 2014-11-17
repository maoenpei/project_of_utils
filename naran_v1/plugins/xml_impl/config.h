/*
 * "$Id: config.h,v 1.1.1.1 2008/12/19 01:08:14 fyang Exp $"
 *
 * Configuration file for Mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003-2007 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * Beginning with VC2005, Microsoft breaks ISO C and POSIX conformance
 * by deprecating a number of functions in the name of security, even
 * when many of the affected functions are otherwise completely secure.
 * The _CRT_SECURE_NO_DEPRECATE definition ensures that we won't get
 * warnings from their use...
 */

#define _CRT_SECURE_NO_DEPRECATE

/*
 * Microsoft also renames the POSIX functions to _name, and introduces
 * a broken compatibility layer using the original names.  As a result,
 * random crashes can occur when, for example, strdup() allocates memory
 * from a different heap than used by malloc() and free().
 *
 * To avoid moronic problems like this, we #define the POSIX function
 * names to the corresponding non-standard Microsoft names.
 */

/*#define close		_close
#define open		_open
#define read	        _read
#define snprintf 	_snprintf
#define strdup		_strdup
#define vsnprintf 	_vsnprintf
#define write		_write*/


/*
 * Include necessary headers...
 */

#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>


/*
 * Version number...
 */

#define MXML_VERSION "Mini-XML v2.4.1"


/*
 * Inline function support...
 */

#define inline _inline


/*
 * Do we have the snprintf() and vsnprintf() functions?
 */

//#define HAVE_SNPRINTF 1
//#define HAVE_VSNPRINTF 1


/*
 * Do we have the strXXX() functions?
 */

//#define HAVE_STRDUP 1


/*
 * Define prototypes for string functions as needed...
 */

#  ifndef HAVE_STRDUP
extern char	*_mxml_strdup(const char *);
#    define strdup _mxml_strdup
#  endif /* !HAVE_STRDUP */

extern char	*_mxml_strdupf(const char *, ...);
extern char	*_mxml_vstrdupf(const char *, va_list);

#  ifndef HAVE_SNPRINTF
extern int	_mxml_snprintf(char *, size_t, const char *, ...);
#    define snprintf _mxml_snprintf
#  endif /* !HAVE_SNPRINTF */

#  ifndef HAVE_VSNPRINTF
extern int	_mxml_vsnprintf(char *, size_t, const char *, va_list);
#    define vsnprintf _mxml_vsnprintf
#  endif /* !HAVE_VSNPRINTF */

/*
 * End of "$Id: config.h,v 1.1.1.1 2008/12/19 01:08:14 fyang Exp $".
 */
