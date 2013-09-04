/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#include "ldap.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <ldap.h>
#include <sasl/sasl.h>

#include <et/com_err.h>

#ifndef __hidden
#  define __hidden __attribute__((__visibility__("hidden")))
#endif

static void xasprintf(struct ensc_ldap_ctx *ctx, char const *fmt, ...)
{
	va_list		ap;
	int		rc;

	if (!ctx)
		return;

	ctx->err_msg = NULL;

	va_start(ap, fmt);
	rc = vasprintf(&ctx->err_msg, fmt, ap);
	va_end(ap);

	if (rc < 0) {
		com_err("ldap", errno, "vasprintf()");
		ctx->err_msg = NULL;
	}
}

static int init_ldap(LDAP *ldp)
{
	int		rc;
	int const	opt_version = 3;
	char const	*opt_mech = "GSSAPI";

	rc = ldap_set_option(ldp, LDAP_OPT_PROTOCOL_VERSION, &opt_version);
	if (rc >= 0 && 0)
		rc = ldap_set_option(ldp, LDAP_OPT_X_SASL_MECH, &opt_mech);
	if (rc >= 0 && 0)
		rc = ldap_set_option(ldp, LDAP_OPT_X_SASL_REALM, "");
	if (rc >= 0 && 0)
		rc = ldap_set_option(ldp, LDAP_OPT_X_SASL_AUTHCID, "");
	if (rc >= 0 && 0)
		rc = ldap_set_option(ldp, LDAP_OPT_X_SASL_AUTHZID, "");

	return rc;
}

static LDAP *open_ldap(char const *ldap_dn, struct ensc_ldap_ctx *ctx)
{
	int		rc;
	char		*ldap_servers = NULL;
	char		*ptr;
	char		*uri = NULL;
	LDAP		*ldp = NULL;
	char		*domain = NULL;

	rc = ldap_dn2domain(ldap_dn, &domain);
	if (rc != LDAP_SUCCESS) {
		xasprintf(ctx, "failed to convert '%s' to dns domain: %s",
			  ldap_dn, ldap_err2string(rc));
		return NULL;
	}

	rc = ldap_domain2hostlist(domain, &ldap_servers);
	free(domain);

	if (rc != LDAP_SUCCESS) {
		xasprintf(ctx, "failed to determine ldap servers: %s",
			  ldap_err2string(rc));
		return NULL;
	}

	ptr = ldap_servers;
	while (ptr) {
		char	*s = strsep(&ptr, " ");
		char	*new_uri;

		new_uri = realloc(uri, (uri ? strlen(uri) : 0)
				  + strlen(s) + sizeof(" ldap://"));
		if (!new_uri) {
			xasprintf(ctx,
				  "failed to allocate memory for ldap uri: %s",
				  strerror(errno));
			ldap_memfree(ldap_servers);
			goto out;
		}

		if (uri)
			strcat(new_uri, " ");
		else
			new_uri[0] = '\0';

		strcat(new_uri, "ldap://");
		strcat(new_uri, s);

		uri = new_uri;
	}
	ldap_memfree(ldap_servers);
	ldap_servers = NULL;

	rc = ldap_initialize(&ldp, uri);
	if (rc != LDAP_SUCCESS) {
		xasprintf(ctx, "ldap_initialize failed: %s",
			  ldap_err2string(rc));
		ldp = NULL;
		goto out;
	}

	if (ctx && ctx->init)
		rc = ctx->init(ldp, ctx->priv);
	else
		rc = init_ldap(ldp);

	if (rc < 0) {
		xasprintf(ctx, "init_ldap(): %s",
			  ldap_err2string(rc));
		ldap_destroy(ldp);
		ldp = NULL;
		goto out;
	}

out:
	free(uri);
	return ldp;
}

static int ldap_bind_fn(LDAP *ldp, unsigned flags, void *priv, void *interact_v)
{
	(void)flags;
	(void)priv;
	(void)interact_v;

	if (!ldp)
		return LDAP_PARAM_ERROR;

	return LDAP_SUCCESS;
}

 __hidden LDAP *ensc_ldap_connect(char const *uri, struct ensc_ldap_ctx *ctx)
{
	LDAP	*ldap;
	int	rc;

	ldap = open_ldap(uri, ctx);
	if (!ldap)
		return NULL;

	rc = ldap_sasl_interactive_bind_s(ldap, NULL, NULL,
					  NULL, NULL, LDAP_SASL_QUIET,
					  ldap_bind_fn, NULL);
	if (rc < 0) {
		xasprintf(ctx, "ldap_sasl_interactive_bind_s(): %s",
			  ldap_err2string(rc));
		goto out;
	}

	rc = 0;

out:
	if (rc < 0) {
		ldap_destroy(ldap);
		ldap = NULL;
	}

	return ldap;
}
