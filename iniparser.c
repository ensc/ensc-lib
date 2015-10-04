/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#include "iniparser.h"

#include <wordexp.h>

#include <iniparser.h>
#include <et/com_err.h>

#include "i18n.h"
#include "xalloc.h"
#include "compiler.h"

static char const *_iniparser_getstring(struct _dictionary_ const *cfg,
					char const *key,
					char const *dflt)
{
	return iniparser_getstring(const_cast(struct _dictionary_ *)(cfg),
				   key,
				   const_cast(char *)(dflt));
}

_hidden_ char const *iniparser_getstring_exp(struct _dictionary_ *cfg,
					     char const *key,
					     char const *dflt)
{
	char const	*tmp = _iniparser_getstring(cfg, key, dflt);
	int		rc;
	wordexp_t	p;
	char const	*msg;

	if (!tmp) {
		rc = -1;
		msg = N_("failed to get string");
	} else {
		rc = wordexp(tmp, &p, WRDE_NOCMD | WRDE_UNDEF);	

		switch (rc) {
		case WRDE_BADCHAR:
			msg = N_("bad char");
			break;

		case WRDE_BADVAL:
			msg = N_("undefined shell variable");
			break;

		case WRDE_CMDSUB:
			msg = N_("command substitution not allowed");
			break;

		case WRDE_NOSPACE:
			msg = N_("out of memory");
			break;

		case WRDE_SYNTAX:
			msg = N_("syntax error");
			break;

		default:
			msg = N_("???");
			break;
		}
	}

	if (rc != 0) {
		com_err(__func__, 0, 
			N_("failed to get '%s' (--> '%s') configuration option: %s\n"),
			key, tmp, msg);
		return NULL;
	}

	if (p.we_wordc != 1) {
		com_err(__func__, 0, 
			N_("ambiguous expansion for '%s' (--> '%s'): %zu values\n"),
			key, tmp, p.we_wordc);
		tmp = NULL;
	} else {
		tmp = Xstrdup(p.we_wordv[0]);
	}

	wordfree(&p);

	return tmp;
}
