/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_UNSERIALZE_H
#define H_ENSC_LIB_UNSERIALZE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "strbuf.h"

struct unserialize_stream {
	void const	*ptr;
	size_t		cnt;
};

__attribute__((__nonnull__(2)))
inline static bool _unserialize_cpy(struct unserialize_stream *src,
				    void  *dst, size_t len)
{
	if (len > src->cnt)
		return false;

	memcpy(dst, src->ptr, len);
	src->cnt -= len;
	src->ptr += len;

	return true;
}

inline static bool unserialize_u32(struct unserialize_stream *src, uint32_t *v)
{
	if (!_unserialize_cpy(src, v, sizeof *v))
		return false;

	*v = be32toh(*v);
	return true;
}

inline static bool unserialize_u64(struct unserialize_stream *src, uint64_t *v)
{
	if (!_unserialize_cpy(src, v, sizeof *v))
		return false;

	*v = be64toh(*v);
	return true;
}

inline static bool unserialize_time_t(struct unserialize_stream *src,
				      time_t *v)
{
	uint64_t	tmp;

	if (!unserialize_u64(src, &tmp))
		return false;

	*v = tmp;
	return true;
}

inline static bool unserialize_buf(struct unserialize_stream *src,
				   void *dst, size_t *dst_len)
{
	uint32_t	l;

	if (!unserialize_u32(src, &l) ||
	    *dst_len < l ||
	    !_unserialize_cpy(src, dst, l))
		return false;

	*dst_len = l;
	return true;
}

inline static bool unserialize_str(struct unserialize_stream *src,
				   char *dst, size_t max_len)
{
	if (max_len == 0)
		return false;

	max_len -= 1;			/* reserve space for trailing '\0' */
	if (!unserialize_buf(src, dst, &max_len))
		return false;

	dst[max_len] = '\0';
	return true;
}

inline static bool unserialize_bufa(struct unserialize_stream *src,
				    struct strbuf *dst, bool optimize)
{
	uint32_t	l;

	if (!unserialize_u32(src, &l) || src->cnt < l)
		return false;

	strbuf_clear(dst);
	strbuf_allocate(dst, l);

	if (l > 0 && !_unserialize_cpy(src, dst->b, l))
		return false;

	dst->len = l;

	if (optimize)
		strbuf_shrink(dst);

	return true;
}

inline static bool unserialize_stra(struct unserialize_stream *src,
				    char const **dst, size_t *dst_len)
{
	struct strbuf	buf;
	bool		rc;


	strbuf_init(&buf, 0);
	rc = unserialize_bufa(src, &buf, false);
	if (rc) {
		strbuf_to_str(&buf, true);
		*dst = strbuf_xfer(&buf, dst_len);
	}

	return rc;
}

#endif	/* H_ENSC_LIB_UNSERIALZE_H */
