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

#include "../safe_calloc.h"

struct test {
	int	a;
	int	b[];
};

int main(void)
{
	int volatile a = 23;
	int volatile b = 42;

	struct test	t;
	size_t		sz;
	size_t volatile c = SIZE_MAX / 2;

	BUG_ON(sizeof_flexarr(&sz, t, c, b) != 0);

	return safe_calloc(a, b) ? 0 : 1;
}
