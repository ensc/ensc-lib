/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#include "ldap.h"

#include <stdio.h>
#include <stdint.h>

#include "xalloc.h"
#include "compiler.h"

#define HEX	"0123456789abcdef"

_hidden_ char const *ldap_escape_query(char const *s, ssize_t cnt_)
{
	size_t const	cnt = cnt_ < 0 ? strlen(s) : (size_t)(cnt_);
	char		*buf = NULL;
	char		*d;
	int		pass;

	for (pass = 0; pass < 2; ++pass) {
		size_t		i;
		size_t		alloc_cnt = 0;

		d = buf;

		for (i = 0; i < cnt; ++i) {
			unsigned char	c = *s++;

			switch (c) {
			case '*':
			case '(':
			case ')':
			case '\\':
			case '\0':
				if (d) {
					/* pass #2 */
					*d++ = '\\';
					*d++ = HEX[(c >> 4) & 0x0f];
					*d++ = HEX[(c >> 0) & 0x0f];
				} else {
					/* pass #1 */
					alloc_cnt += 3;
				}
				break;

			default:
				if (d) {
					/* pass #2 */
					*d++ = c;
				} else {
					/* pass #1 */
					alloc_cnt += 1;
				}
				break;
			}
		}

		if (pass == 0)
			buf = Xmalloc(alloc_cnt + 1);
	}

	*d++ = '\0';

	return buf;
}

_hidden_ char const *ldap_error_string(LDAP *ldp)
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
