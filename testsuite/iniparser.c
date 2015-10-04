/*	--*- c -*--
 * Copyright (C) 2015 Enrico Scholz <enrico.scholz@ensc.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "../iniparser.h"

#include <stdlib.h>
#include <errno.h>

#include <et/com_err.h>

void alloc_error(char const *src_func, char const *file, unsigned int line,
		 char const *alloc_func)
{
	com_err(src_func, ENOMEM, "%s:%u %s() failed", file, line, alloc_func);
	abort();
}

int main(void)
{
}
