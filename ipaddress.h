/*	--*- c -*--
 * Copyright (C) 2015 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@
 */

#ifndef H_ENSC_LIB_IPADDRESS_H
#define H_ENSC_LIB_IPADDRESS_H

struct addrinfo;
void const *ipaddress_parse(char const *uri,
			    struct addrinfo *info,
			    char const **node,
			    char const **service);

#endif	/* H_ENSC_LIB_IPADDRESS_H */
