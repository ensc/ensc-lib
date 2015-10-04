/*	--*- c -*--
 * Copyright (C) 2015 Enrico Scholz <enrico.scholz@ensc.de>
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

#include "../ipaddress.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

#include "../xalloc.h"

static int streq(char const *a, char const *b)
{
	if (a == b)
		return 1;
	else if (!a && b)
		return 0;
	else if (a && !b)
		return 0;
	else
		return strcmp(a, b) == 0;
}

#define TEST(_uri, _family, _node, _service) do {			\
		struct addrinfo		info;				\
		char const		*node;				\
		char const		*service;			\
		void const		*tmp;				\
									\
		tmp = ipaddress_parse(_uri, &info, &node, &service);	\
									\
		assert(tmp);						\
		assert(info.ai_family == (_family));			\
		assert(streq(node, _node));				\
		assert(streq(service, _service));			\
									\
		freec(tmp);						\
	} while (0)

int main(void)
{
	TEST("ip4://example.com:1234",  AF_INET,  "example.com",  "1234");
	TEST("ip4://example.com:1234/", AF_INET,  "example.com",  "1234");
	TEST("ip4://example.com",       AF_INET,  "example.com",  NULL);
	TEST("ip4://example.com/",      AF_INET,  "example.com",  NULL);
	TEST("ip6://example.com:1234",  AF_INET6, "example.com",  "1234");
	TEST("ip6://[1::0]:1234",       AF_INET6, "1::0",         "1234");
	TEST("ip6://[1::0]",            AF_INET6, "1::0",         NULL);
	TEST("ip6://[1::0]/",           AF_INET6, "1::0",         NULL);
	TEST("ip://example.com",        AF_UNSPEC, "example.com", NULL);
	TEST("ip://example.com:1234",   AF_UNSPEC, "example.com", "1234");
	TEST("unix:///abc",             AF_UNIX,   "/abc",        NULL);
	TEST("unix:///abc:1234",        AF_UNIX,   "/abc:1234",   NULL);

	return 0;
}
