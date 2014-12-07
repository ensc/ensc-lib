/*	--*- c -*--
 * Copyright (C) 2014 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
 @PROJECT_LICENSE@

 Most part of this file were taken from systemd's sd-daemon.c
 */

#ifndef DISABLE_SYSTEMD

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif

#include "sd-notify.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "compiler.h"

_hidden_ int sd_notifyf(int unset_environment, const char *format, ...) 
{
        va_list ap;
        char *p = NULL;
        int r;

        va_start(ap, format);
        r = vasprintf(&p, format, ap);
        va_end(ap);

        if (r < 0 || !p)
                return -ENOMEM;

        r = sd_notify(unset_environment, p);
        free(p);

        return r;
}

#endif	/* DISABLE_SYSTEMD */
