/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_I18N_H
#define H_ENSC_LIB_I18N_H

#include <libintl.h>

#define _(String)		gettext(String)
#define gettext_noop(String)	(String)
#define N_(String)		gettext_noop(String)

#endif	/* H_ENSC_LIB_I18N_H */
