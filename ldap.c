/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#include "ldap.h"

#include <stdio.h>

#include "xalloc.h"

#ifndef __hidden
#  define __hidden __attribute__((__visibility__("hidden")))
#endif

#define HEX	"0123456789abcdef"

__hidden char const *ldap_escape_query(char const *s, ssize_t cnt)
{
	char		*buf;
	char		*d;

	if (cnt == -1)
		cnt = strlen(s);

	buf = Xmalloc(cnt * 3 + 1);

	d = buf;
	while (cnt-- > 0) {
		unsigned char	c = *s++;

		switch (c) {
		case '*':
		case '(':
		case ')':
		case '\\':
		case '\0':
			*d++ = '\\';
			*d++ = HEX[(c >> 4) & 0x0f];
			*d++ = HEX[(c >> 0) & 0x0f];
			break;

		default:
			*d++ = c;
			break;
		}
	}

	*d++ = '\0';
	buf = Xrealloc(buf, d - buf);

	return buf;
}

__hidden char const *ldap_error_string(LDAP *ldp)
{
	char	*res;
	char	*msg = NULL;
	int	rc = ldap_get_option(ldp, LDAP_OPT_ERROR_STRING, &msg);

	if (rc >= 0) {
		res = strdup(msg);
		ldap_memfree(msg);
	} else if (asprintf(&res,
			    "ldap_get_option(<LDAP_OPT_ERROR_STRING>): %s",
			    ldap_err2string(rc)) == 0) {
		;			/* noop */
	} else {
		res = NULL;
	}

	return res;
}
