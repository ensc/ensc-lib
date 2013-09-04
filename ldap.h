/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_LDAP_H
#define H_ENSC_LIB_LDAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <ldap.h>

struct ensc_ldap_ctx {
	int	(*init)(LDAP *ldp, void *);

	char	*err_msg;
	void	*priv;
};

LDAP *ensc_ldap_connect(char const *uri, struct ensc_ldap_ctx *ctx);
char const *ldap_escape_query(char const *s, ssize_t cnt);
char const *ldap_error_string(LDAP *ldp);

#define ldapm_escape_query(_pool, _s, _cnt)			\
	mgmmem_xfer(_pool, ldap_escape_query(_s, _cnt))

#define ldapm_error_string(_pool, _ldp) \
	mgmmem_xfer(_pool, ldap_error_string(_ldp))


#ifdef NO_ldap_destroy
inline static int ldap_destroy(LDAP *ldp)
{
	return ldap_unbind_ext_s(ldp, NULL, NULL);
}
#endif

#endif	/* H_ENSC_LIB_LDAP_H */
