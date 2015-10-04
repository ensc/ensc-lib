/*	--*- c -*--
 * Copyright (C) 2015 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#include "ipaddress.h"

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/ip.h>

#include <et/com_err.h>

#include "compiler.h"

_hidden_ void const *ipaddress_parse(char const *uri,
				     struct addrinfo *info,
				     char const **node,
				     char const **service)
{
	char		*res = strdup(uri);
	char		*ptr = res;

	if (!res)
		return NULL;

	memset(info, 0, sizeof *info);

	if (memcmp(ptr, "ip4://", 6) == 0) {
		info->ai_family = AF_INET;
		ptr += 6;
	} else if (memcmp(ptr, "ip6://", 6) == 0) {
		info->ai_family = AF_INET6;
		ptr += 6;
	} else if (memcmp(ptr, "ip://", 5) == 0) {
		info->ai_family = AF_UNSPEC;
		ptr += 5;
	} else if (memcmp(ptr, "unix://", 7) == 0) {
		info->ai_family = AF_UNIX;
		ptr += 7;
	}  else {
		com_err("ipaddress_parse", EINVAL,
			"unsupported address family '%s'", ptr);
		goto err;
	}

	*node = ptr;

	if (info->ai_family == AF_INET6 && **node == '[') {
		++*node;
		ptr = strchr(*node, ']');

		if (ptr) {
			*ptr = '\0';
			++ptr;
		} else {
			com_err("ipaddress_parse", EINVAL,
				"unterminated ipv6 bracket address '%s'", uri);
			goto err;
		}
	}

	switch (info->ai_family) {
	case AF_INET:
	case AF_INET6:
	case AF_UNSPEC:
		ptr = strchr(ptr, ':');
		if (!ptr) {
			*service = NULL;
		} else {
			*ptr++   = '\0';
			*service = ptr;
		}
		break;

	case AF_UNIX:
		*service = NULL;
		break;

	default:
		BUG();
	}

	if (*node && info->ai_family != AF_UNIX) {
		ptr = strchr(*node, '/');
		if (ptr)
			*ptr = '\0';
	}

	if (*service) {
		ptr = strchr(*service, '/');
		if (ptr)
			*ptr = '\0';
	}

	return res;

err:
	free(res);
	return NULL;
}
