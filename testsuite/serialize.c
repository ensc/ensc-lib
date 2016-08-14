/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define UNSERIALIZE_SUPPORT_MEM		1
#define UNSERIALIZE_SUPPORT_SOCKET	0
#define UNSERIALIZE_SUPPORT_FD		0
#define UNSERIALIZE_SUPPORT_GENERIC	0

#include "../unserialize.h"
#include "../serialize.h"

#include <stdio.h>

#define BUF0	""
#define BUF1	"buf1"
#define BUF2	"buf2abcdefg"
#define BUF3	"buf3ZY"

#define STR0	""
#define STR1	"str1"
#define STR2	"str2ABCDEFG"
#define STR3	"str3zy"

void alloc_error(char const *src_func, char const *file, unsigned int line,
		 char const *alloc_func)
{
	fprintf(stderr, "%s:%u/%s %s() failed", file, line, src_func, alloc_func);
	abort();
}

static void test0(struct unserialize_stream_mem *u)
{
	char		buf[128];
	size_t		l = sizeof buf;

#define Xunserialize_buf(_u, _buf, _exp)		\
	l = sizeof _buf - 1;				\
	memset(_buf, '\1', sizeof _buf);		\
	assert(unserialize_buf(_u, _buf+1, &l));	\
	assert(l == sizeof _exp - 1);			\
	assert(memcmp(_buf+1, _exp, l) == 0);		\
	assert(memcmp(_buf, "\1", 1) == 0);		\
	assert(memcmp(_buf+1+l, "\1", 1) == 0);		\

#define Xunserialize_str(_u, _buf, _exp)			\
	l = sizeof _buf - 1;					\
	memset(_buf, '\1', sizeof _buf);			\
	assert(unserialize_str(_u, _buf+1, l));			\
	assert(strcmp(_buf+1, _exp) == 0);			\
	assert(memcmp(_buf+1+strlen(_buf), "\1", 1) == 0);	\

	Xunserialize_buf(&u->s, buf, BUF0);
	Xunserialize_buf(&u->s, buf, BUF1);
	Xunserialize_buf(&u->s, buf, BUF2);
	Xunserialize_buf(&u->s, buf, BUF3);

	Xunserialize_str(&u->s, buf, STR0);
	Xunserialize_str(&u->s, buf, STR1);
	Xunserialize_str(&u->s, buf, STR2);
	Xunserialize_str(&u->s, buf, STR3);

#undef Xunserialize_str
#undef Xunserialize_buf

	assert(u->cnt == 0);
}

static void test1(struct unserialize_stream_mem *u)
{
	char				buf[128];
	size_t				l = sizeof buf;


#define Xunserialize_buf(_u, _buf, _exp)			\
	if (sizeof _exp > 1) {					\
		struct unserialize_stream_mem tmp = *u;		\
		l = sizeof _exp - 2;				\
		assert(!unserialize_buf(&tmp.s, _buf, &l));	\
	}							\
	l = sizeof _exp - 1;					\
	memset(_buf, '\1', sizeof _buf);			\
	assert(unserialize_buf(_u, _buf+1, &l));		\
	assert(l == sizeof _exp - 1);				\
	assert(memcmp(_buf+1, _exp, l) == 0);			\
	assert(memcmp(_buf, "\1", 1) == 0);			\
	assert(memcmp(_buf+1+l, "\1", 1) == 0);			\

#define Xunserialize_str(_u, _buf, _exp)			\
	l = sizeof _exp;					\
	{							\
		struct unserialize_stream_mem tmp = *u;		\
		assert(!unserialize_str(&tmp.s, buf, l-1));	\
	}							\
	memset(_buf, '\1', sizeof _buf);			\
	assert(unserialize_str(_u, _buf+1, l));			\
	assert(strcmp(_buf+1, _exp) == 0);			\
	assert(memcmp(_buf+1+strlen(_buf), "\1", 1) == 0);	\

	Xunserialize_buf(&u->s, buf, BUF0);
	Xunserialize_buf(&u->s, buf, BUF1);
	Xunserialize_buf(&u->s, buf, BUF2);
	Xunserialize_buf(&u->s, buf, BUF3);

	Xunserialize_str(&u->s, buf, STR0);
	Xunserialize_str(&u->s, buf, STR1);
	Xunserialize_str(&u->s, buf, STR2);
	Xunserialize_str(&u->s, buf, STR3);

#undef Xunserialize_str
#undef Xunserialize_buf

	assert(u->cnt == 0);
}


static void test2(struct unserialize_stream_mem *u, bool optimize)
{
	struct strbuf	buf = INIT_STRBUF(buf);
	char const	*str;
	size_t		str_len;

#define Xunserialize_bufa(_u, _dst, _exp)			\
	assert(unserialize_bufa(_u, _dst, optimize));		\
	assert((_dst)->len == sizeof _exp - 1);			\
	assert(memcmp((_dst)->b, _exp, (_dst)->len) == 0);	\

#define Xunserialize_stra(_u, _buf, _len, _exp)		\
	assert(unserialize_stra(_u, &(_buf), _len));	\
	assert(strcmp(_buf, _exp) == 0);		\
	assert(*(_len) == sizeof _exp - 1);		\
	freec(_buf);					\

	Xunserialize_bufa(&u->s, &buf, BUF0);
	Xunserialize_bufa(&u->s, &buf, BUF1);
	Xunserialize_bufa(&u->s, &buf, BUF2);
	Xunserialize_bufa(&u->s, &buf, BUF3);

	Xunserialize_stra(&u->s, str, &str_len, STR0);
	Xunserialize_stra(&u->s, str, &str_len, STR1);
	Xunserialize_stra(&u->s, str, &str_len, STR2);
	Xunserialize_stra(&u->s, str, &str_len, STR3);

#undef Xunserialize_str
#undef Xunserialize_buf

	assert(u->cnt == 0);

	strbuf_destroy(&buf);
}

int main(void)
{
	struct serialize_stream		s = INIT_SERIALIZE_STREAM(s);
	struct unserialize_stream_mem	u0;
	struct unserialize_stream_mem	u1;

	serialize_u32(&s, 23);
	serialize_u64(&s, 42);
	serialize_time_t(&s, 1);
	serialize_buf(&s, BUF0, sizeof BUF0 - 1);
	serialize_buf(&s, BUF1, sizeof BUF1 - 1);
	serialize_buf(&s, BUF2, sizeof BUF2 - 1);
	serialize_buf(&s, BUF3, sizeof BUF3 - 1);

	serialize_allocate(&s, 80);

	serialize_str(&s, STR0);
	serialize_str(&s, STR1);
	serialize_str(&s, STR2);
	serialize_str(&s, STR3);

	u0.s.type = UNSERIALIZE_SRC_MEM;
	u0.ptr = s.base;
	u0.cnt = s.cnt;

	{
		uint32_t	v;
		assert(unserialize_u32(&u0.s, &v) && v == 23);
	}

	{
		uint64_t	v;
		assert(unserialize_u64(&u0.s, &v) && v == 42);
	}

	{
		time_t		v;
		assert(unserialize_time_t(&u0.s, &v) && v == 1);
	}

	u1 = u0;

	u0 = u1;
	test0(&u0);

	u0 = u1;
	test1(&u0);

	u0 = u1;
	test2(&u0, false);

	u0 = u1;
	test2(&u0, true);

	serialize_destroy(&s, true);

	return 0;
}
