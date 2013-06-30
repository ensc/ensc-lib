/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */
#ifndef H_ENSC_LIB_IO_H
#define H_ENSC_LIB_IO_H

#include <stdbool.h>
#include <stdlib.h>

bool read_all(int fd, void *dst, size_t len);
bool read_str(int fd, char *dst, size_t max_len);

bool write_all(int fd, void const *src, size_t len);
bool write_str(int fd, char const *src, ssize_t len);

#endif	/* H_ENSC_LIB_IO_H */
