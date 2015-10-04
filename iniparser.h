/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_INIPARSER_H
#define H_ENSC_LIB_INIPARSER_H

#include <stdbool.h>
#include <sys/types.h>

#include "compiler.h"

struct _dictionary_;
char const *iniparser_getstring_exp(struct _dictionary_ const *cfg,
				    char const *key,
				    char const *dflt) _malloc_;

bool iniparser_getuser(struct _dictionary_ const *cfg, uid_t *uid, gid_t *gid,
		       char const *key, bool is_critical);

bool iniparser_getgroup(struct _dictionary_ const *cfg, gid_t *gid,
			char const *key, bool is_critical);

#endif	/* H_ENSC_LIB_INIPARSER_H */
