/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#include "mgmmem.h"

#include <stdarg.h>
#include <stdio.h>

#include "compiler.h"

#include "xalloc.h"
#include "compiler.h"

struct mgmmem_entry {
	void const	*ptr;
};

_hidden_ void mgmmem_destroy(struct mgmmem_pool *pool)
{
	size_t		i;

	for (i = pool->num_entries; i > 0; --i)
		freec(pool->entries[i-1].ptr);

	free(pool->entries);
}

_hidden_ bool _mgmmem_xfer(struct mgmmem_pool *pool, void const *ptr)
{
	struct mgmmem_entry	*tmp;

	if (!ptr) {
		pool->flags |= MGMMEM_POOL_NULL_PTR;
		return true;
	}

	if (pool->num_entries < pool->cnt_allocated) {
		tmp = pool->entries;
	} else {
		size_t	new_cnt = pool->num_entries * 150 / 100 + 2;

		tmp = Xrealloc(pool->entries, new_cnt * sizeof *tmp);

		pool->entries = tmp;
		pool->cnt_allocated = new_cnt;
	}

	if (!tmp) {
		freec(ptr);
		pool->flags |= MGMMEM_POOL_BAD_ALLOC;
	} else {
		tmp += pool->num_entries;
		pool->num_entries += 1;
		tmp->ptr = ptr;
	}

	return tmp != NULL;
}

_hidden_ char *mgmmem_printf(struct mgmmem_pool *pool, char const *fmt, ...)
{
	va_list		ap;
	int		rc;
	char		*buf;

	va_start(ap, fmt);
	rc = vasprintf(&buf, fmt, ap);
	va_end(ap);

	if (rc < 0) {
		pool->flags |= MGMMEM_POOL_BAD_ALLOC;
		return NULL;
	}

	return mgmmem_xfer(pool, buf);
}
