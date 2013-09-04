/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_MGMEM_H
#define H_ENSC_LIB_MGMEM_H

#include <stdlib.h>
#include <stdbool.h>

#define MGMMEM_POOL_BAD_ALLOC	(1 << 0)
#define MGMMEM_POOL_NULL_PTR	(1 << 1)

struct mgmmem_entry;
struct mgmmem_pool {
	struct mgmmem_entry	*entries;
	size_t			num_entries;
	size_t			cnt_allocated;
	unsigned long		flags;
};

#define DECLARE_MGMMEM_POOL(_p) { NULL, 0, 0, 0 }

inline static void mgmmem_init(struct mgmmem_pool *pool)
{
	pool->entries = NULL;
	pool->num_entries = 0;
	pool->cnt_allocated = 0;
	pool->flags = 0;
}

void mgmmem_destroy(struct mgmmem_pool *pool);

bool _mgmmem_xfer(struct mgmmem_pool *pool, void const *ptr);

char *mgmmem_printf(struct mgmmem_pool *pool, char *fmt, ...)
	__attribute__((__format__(printf, 2, 3)));

#define mgmmem_xfer(_pool, _ptr) \
	({							\
		__typeof__(_ptr)	_tmp = (_ptr);		\
		if (!_mgmmem_xfer(_pool, _tmp))			\
			_tmp = NULL;				\
		_tmp;						\
	})

#endif	/* H_ENSC_LIB_MGMEM_H */
