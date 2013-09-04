/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_SERIALZE_H
#define H_ENSC_LIB_SERIALZE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "compiler.h"
#include "xalloc.h"

struct serialize_stream {
	void		*base;
	size_t		cnt;
	size_t		allocated;
};

#define INIT_SERIALIZE_STREAM(_name) { NULL, 0, 0 }

inline static void serialize_allocate(struct serialize_stream *dst, size_t len)
{
	if (dst->cnt + len > dst->allocated) {
		size_t	l = dst->cnt + len;

		dst->base = Xrealloc(dst->base, l);
		dst->allocated = l;
	}
}

inline static void serialize_destroy(struct serialize_stream *dst,
				     bool invalidate)
{
	if (invalidate && dst->base) {
		memset(dst->base, 0, dst->cnt);
		barrier();
	}

	free(dst->base);
}

inline static void _serialize_cpy(struct serialize_stream *dst,
				  void const *src, size_t len)
{
	serialize_allocate(dst, len);
	memcpy(dst->base + dst->cnt, src, len);
	dst->cnt += len;
}

inline static void serialize_u32(struct serialize_stream *dst, uint32_t v)
{
	v = htobe32(v);
	_serialize_cpy(dst, &v, sizeof v);
}

inline static void serialize_u64(struct serialize_stream *dst, uint64_t v)
{
	v = htobe64(v);
	_serialize_cpy(dst, &v, sizeof v);
}

inline static void serialize_time_t(struct serialize_stream *dst, time_t v)
{
	serialize_u64(dst, v);
}

inline static void serialize_buf(struct serialize_stream *dst,
				 void const *src, size_t len)
{
	serialize_u32(dst, len);
	_serialize_cpy(dst, src, len);
}

inline static void serialize_str(struct serialize_stream *dst,
				 char const *src)
{
	if (!src)
		src = "";

	serialize_buf(dst, src, strlen(src));
}

#endif	/* H_ENSC_LIB_SERIALZE_H */
