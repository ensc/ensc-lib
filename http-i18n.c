/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#include "http-i18n.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>

#include "compiler.h"

#include "xalloc.h"
#include "compiler.h"

static struct {
	char const	*key;
	char const	*val;
} const		LANGUAGES[] = {
	{ "de.UTF-8",    "de_DE.UTF-8" },
	{ "de-DE.UTF-8", "de_DE.UTF-8" },
};

static bool is_valid_lang(char const *lang, size_t len)
{
	size_t		i;

	for (i = 0; i < len; ++i) {
		char	c = lang[i];

		if ((c >= '0' && c <= '9') ||
		    (c >= 'a' && c <= 'z') ||
		    (c >= 'A' && c <= 'Z'))
			continue;

		if (i == 0)
			return false;

		if (c == '-' || c == '_')
			continue;

		return false;
	}

	return true;
}

_hidden_ void http_set_locale(char const *lang, char const *domain,
			      char const *localedir)
{
	char const	*i18n_teststr = "";
	char		*lcbuf = NULL;

	textdomain(domain);
	bindtextdomain(domain, localedir);

	while (lang && *lang) {
		char const	*ptr;
		char const	*opts;
		char const	*cur = lang;
		size_t		i;
		char const	*lc;

		ptr = strchr(lang, ',');
		if (ptr == NULL) {
			ptr  = cur + strlen(cur);
			lang = ptr;
		} else {
			lang = ptr + 1;
		}

		opts = memchr(cur, ';', ptr - cur);
		if (opts)
			ptr = opts;

		if (!is_valid_lang(cur, ptr - cur))
			continue;

		lcbuf = Xrealloc(lcbuf, ptr - cur + sizeof(".UTF-8"));

		strncpy(lcbuf, cur, ptr - cur);
		strcpy(lcbuf + (ptr - cur), ".UTF-8");

		lc = lcbuf;
		for (i = 0; i < ARRAY_SIZE(LANGUAGES); ++i) {
			if (strcasecmp(LANGUAGES[i].key, lcbuf) == 0) {
				lc = LANGUAGES[i].val;
				break;
			}
		}

		if (!setlocale(LC_ALL, lc) ||
		    /* test whether pointer are equal; gettext() will return
		     * argument for unsupported languages */
		    dgettext(domain, i18n_teststr) == i18n_teststr)
			continue;

		break;
	}

	free(lcbuf);
}
