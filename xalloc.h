/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_XALLOC_H
#define H_ENSC_LIB_XALLOC_H

#include "compiler.h"

_noreturn_
void alloc_error(char const *, char const *, unsigned int, char const *);

#define _alloc_fn(_type, _fn, ...) __extension__			\
	 ({								\
		 _type	_tmp = _fn(__VA_ARGS__);			\
		 if (unlikely(_tmp == NULL))				\
			 alloc_error(__func__, __FILE__, __LINE__, #_fn); \
		 (_tmp);						\
	 })

#define Xmalloc(_sz)		_alloc_fn(void *, malloc, _sz)
#define Xcalloc(_n,_sz)		_alloc_fn(void *, safe_calloc, _n, _sz)
#define Xrealloc(_ptr,_sz)	_alloc_fn(void *, realloc, _ptr, _sz)
#define Xstrndup(_str, _sz)	_alloc_fn(char *, strndup, _str, _sz)
#define Xstrdup(_str)		_alloc_fn(char *, strdup, _str)

#define freec(_c) __extension__ \
	({ void const *_tmp = (_c); free((void *)_tmp); })

#define Xrecalloc(_ptr,_n,_sz)	_alloc_fn(void *, recalloc, _ptr, _n, _sz)

#define Xmalloc_flexarr(_s, _n, _attr) \
	_alloc_fn(void *, malloc_flexarr, _s, _n, _attr)
#define Xzalloc_flexarr(_s, _n, _attr) \
	_alloc_fn(void *, zalloc_flexarr, _s, _n, _attr)
#define Xrealloc_flexarr(_s, _n, _attr)			\
	_alloc_fn(void *, realloc_flexarr, _s, _n, _attr)

#define xfer_ptr(_ptr)	({ __typeof__(_ptr) _t = (_ptr); (_ptr) = NULL; _t; })

#endif	/* H_ENSC_LIB_XALLOC_H */
