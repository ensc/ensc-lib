/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_UNSERIALZE_H
#define H_ENSC_LIB_UNSERIALZE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "compiler.h"
#include "strbuf.h"

#ifndef UNSERIALIZE_SUPPORT_MEM
#  define UNSERIALIZE_SUPPORT_MEM	1
#endif

#ifndef UNSERIALIZE_SUPPORT_SOCKET
#  define UNSERIALIZE_SUPPORT_SOCKET	0
#endif

#ifndef UNSERIALIZE_SUPPORT_FD
#  define UNSERIALIZE_SUPPORT_FD	0
#endif

#ifndef UNSERIALIZE_SUPPORT_GENERIC
#  define UNSERIALIZE_SUPPORT_GENERIC	1
#endif

#if UNSERIALIZE_SUPPORT_MEM
#  include <string.h>
#endif

#if UNSERIALIZE_SUPPORT_SOCKET
#  include <errno.h>
#  include <sys/socket.h>
#  include <et/com_err.h>
#endif

#if UNSERIALIZE_SUPPORT_FD
#  include <errno.h>
#  include <unistd.h>
#  include <et/com_err.h>
#endif

struct unserialize_stream {
	enum {
#if UNSERIALIZE_SUPPORT_MEM
		UNSERIALIZE_SRC_MEM,
#endif
#if UNSERIALIZE_SUPPORT_SOCKET
		UNSERIALIZE_SRC_SOCKET,
#endif
#if UNSERIALIZE_SUPPORT_FD
		UNSERIALIZE_SRC_FD,
#endif
#if UNSERIALIZE_SUPPORT_GENERIC
		UNSERIALIZE_SRC_GENERIC
#endif
	}	type;
};

struct unserialize_stream_mem {
	struct unserialize_stream	s;
	void const			*ptr;
	size_t				cnt;
};

struct unserialize_stream_fd {
	struct unserialize_stream	s;
	int				fd;
};

struct unserialize_stream_generic {
	struct unserialize_stream	s;
	bool	(*cpy)(struct unserialize_stream *s, void *dst, size_t len);
};

#if UNSERIALIZE_SUPPORT_MEM
inline static bool _unserialize_cpy_mem(struct unserialize_stream *src_,
					void  *dst, size_t len)
{
	struct unserialize_stream_mem	*src =
		container_of(src_, struct unserialize_stream_mem, s);

	if (len > src->cnt)
		return false;

	memcpy(dst, src->ptr, len);
	src->cnt -= len;
	src->ptr += len;

	return true;
}
#endif

#if UNSERIALIZE_SUPPORT_SOCKET
inline static bool _unserialize_cpy_socket(struct unserialize_stream *src_,
					   void  *dst, size_t len)
{
	struct unserialize_stream_fd	*src =
		container_of(src_, struct unserialize_stream_fd, s);

	while (len > 0) {
		ssize_t	l = recv(src->fd, dst, len, MSG_WAITALL);

		if (l > 0) {
			len -= l;
			dst += l;
		} else if (l == 0) {
			break;
		} else if (errno == EINTR) {
			continue;
		} else if (errno == EAGAIN) {
			fd_set	fds;

			FD_ZERO(&fds);
			FD_SET(src->fd, &fds);

			select(src->fd + 1, &fds, NULL, NULL, NULL);
		} else {
			com_err(__func__, errno, "recv()");
			break;
		}
	}

	return len == 0;
}
#endif

#if UNSERIALIZE_SUPPORT_FD
inline static bool _unserialize_cpy_fd(struct unserialize_stream *src_,
					   void  *dst, size_t len)
{
	struct unserialize_stream_fd	*src =
		container_of(src_, struct unserialize_stream_fd, s);

	while (len > 0) {
		ssize_t	l = read(src->fd, dst, len);

		if (l > 0) {
			len -= l;
			dst += l;
		} else if (l == 0) {
			break;
		} else if (errno == EINTR) {
			continue;
		} else if (errno == EAGAIN) {
			fd_set	fds;

			FD_ZERO(&fds);
			FD_SET(src->fd, &fds);

			select(src->fd + 1, &fds, NULL, NULL, NULL);
		} else {
			com_err(__func__, errno, "read()");
			break;
		}
	}

	return len == 0;
}
#endif

#if UNSERIALIZE_SUPPORT_GENERIC
inline static bool _unserialize_cpy_generic(struct unserialize_stream *src_,
					    void  *dst, size_t len)
{
	struct unserialize_stream_generic	*src =
		container_of(src_, struct unserialize_stream_generic, s);

	return src->cpy(src_, dst, len);
}
#endif

__attribute__((__nonnull__(2)))
inline static bool _unserialize_cpy(struct unserialize_stream *src,
				    void  *dst, size_t len)
{
	switch (src->type) {
#if UNSERIALIZE_SUPPORT_MEM
	case UNSERIALIZE_SRC_MEM:
		return _unserialize_cpy_mem(src, dst, len);
#endif
#if UNSERIALIZE_SUPPORT_SOCKET
	case UNSERIALIZE_SRC_SOCKET:
		return _unserialize_cpy_socket(src, dst, len);
#endif
#if UNSERIALIZE_SUPPORT_FD
	case UNSERIALIZE_SRC_FD:
		return _unserialize_cpy_fd(src, dst, len);
#endif
#if UNSERIALIZE_SUPPORT_GENERIC
	case UNSERIALIZE_SRC_GENERIC:
		return _unserialize_cpy_generic(src, dst, len);
#endif
	default:
		BUG();
	}
}

inline static bool _unserialize_is_available(struct unserialize_stream *src,
					     size_t l)
{
	switch (src->type) {
#if UNSERIALIZE_SUPPORT_MEM
	case UNSERIALIZE_SRC_MEM: {
		struct unserialize_stream_mem	*s =
			container_of(src, struct unserialize_stream_mem, s);

		return s->cnt >= l;
	}
#endif
#if UNSERIALIZE_SUPPORT_FD
	case UNSERIALIZE_SRC_FD:
		return true;
#endif
#if UNSERIALIZE_SUPPORT_SOCKET
	case UNSERIALIZE_SRC_SOCKET:
		return true;
#endif
#if UNSERIALIZE_SUPPORT_GENERIC
	case UNSERIALIZE_SRC_GENERIC:
		return true;
#endif
	default:
		BUG();
	}
}

inline static bool unserialize_u16(struct unserialize_stream *src, uint16_t *v)
{
	if (!_unserialize_cpy(src, v, sizeof *v))
		return false;

	*v = be16toh(*v);
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

	if (!unserialize_u32(src, &l) ||
	    !_unserialize_is_available(src, l))
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
