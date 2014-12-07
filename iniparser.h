/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_INIPARSER_H
#define H_ENSC_LIB_INIPARSER_H

struct _dictionary_;
char const *iniparser_getstring_exp(struct _dictionary_ *cfg,
				    char const *key,
				    char const *dflt);

#endif	/* H_ENSC_LIB_INIPARSER_H */
