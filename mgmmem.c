/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#include "mgmmem.h"

#include <stdarg.h>
#include <stdio.h>

#include "compiler.h"

#ifndef __hidden
#  define __hidden __attribute__((__visibility__("hidden")))
#endif

struct mgmmem_entry {
	void const	*ptr;
};

 __hidden void mgmmem_destroy(struct mgmmem_pool *pool)
{
	size_t		i;

	for (i = pool->num_entries; i > 0; --i)
		free(const_cast(void *)(pool->entries[i-1].ptr));

	free(pool->entries);
}

__hidden bool _mgmmem_xfer(struct mgmmem_pool *pool, void const *ptr)
{
	struct mgmmem_entry	*tmp;

	if (!ptr) {
		pool->flags |= MGMMEM_POOL_NULL_PTR;
		return NULL;
	}

	if (pool->num_entries < pool->cnt_allocated) {
		tmp = pool->entries;
	} else {
		size_t	new_cnt = pool->num_entries * 150 / 100 + 2;

		tmp = realloc(pool->entries, new_cnt * sizeof *tmp);

		if (tmp) {
			pool->entries = tmp;
			pool->cnt_allocated = new_cnt;
		}
	}

	if (!tmp) {
		free(const_cast(void *)(ptr));
		pool->flags |= MGMMEM_POOL_BAD_ALLOC;
	} else {
		tmp += pool->num_entries;
		pool->num_entries += 1;
		tmp->ptr = ptr;
	}

	return tmp != NULL;
}

__hidden char *mgmmem_printf(struct mgmmem_pool *pool, char *fmt, ...)
{
	va_list		ap;
	int		rc;
	char		*buf;

	va_start(ap, fmt);
	rc = vasprintf(&buf, fmt, ap);
	va_end(ap);

	if (rc < 0)
		buf = NULL;

	return mgmmem_xfer(pool, buf);
}
