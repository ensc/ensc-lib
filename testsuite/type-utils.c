/*	--*- c -*--
 * Copyright (C) 2016 Enrico Scholz <enrico.scholz@ensc.de>
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

#include "../type-utils.h"
#include <stdint.h>
#include <limits.h>

int main(void)
{
	BUG_ON(variable_max_value((unsigned char)0)  != UCHAR_MAX);
	BUG_ON(variable_max_value((unsigned char const)0)  != UCHAR_MAX);
	BUG_ON(variable_max_value((unsigned short)0) != USHRT_MAX);
	BUG_ON(variable_max_value((unsigned int)0)   != UINT_MAX);
	BUG_ON(variable_max_value((unsigned long)0)  != ULONG_MAX);
	BUG_ON(variable_max_value((unsigned long long)0)  != ULLONG_MAX);

	BUG_ON(variable_max_value((unsigned char)0) >
	       variable_max_value((unsigned short)0));

	BUG_ON(variable_max_value((unsigned short)0) >
	       variable_max_value((unsigned int)0));

	BUG_ON(variable_max_value((unsigned int)0) >
	       variable_max_value((unsigned long)0));

	BUG_ON(variable_max_value((unsigned long)0) >
	       variable_max_value((unsigned long long)0));

	/*  */

	BUG_ON(variable_max_value((signed char)0)  != CHAR_MAX);
	BUG_ON(variable_max_value((signed short)0) != SHRT_MAX);
	BUG_ON(variable_max_value((signed int)0)   != INT_MAX);
	BUG_ON(variable_max_value((signed long)0)  != LONG_MAX);
	BUG_ON(variable_max_value((signed long long)0)  != LLONG_MAX);

	BUG_ON(variable_max_value((signed char)0) >
	       variable_max_value((signed short)0));

	BUG_ON(variable_max_value((signed short)0) >
	       variable_max_value((signed int)0));

	BUG_ON(variable_max_value((signed int)0) >
	       variable_max_value((signed long)0));

	BUG_ON(variable_max_value((signed long)0) >
	       variable_max_value((signed long long)0));

	/*  */
	BUG_ON(variable_min_value((unsigned char)0)  != 0);
	BUG_ON(variable_min_value((unsigned short)0) != 0);
	BUG_ON(variable_min_value((unsigned int)0)   != 0);
	BUG_ON(variable_min_value((unsigned long)0)  != 0);
	BUG_ON(variable_min_value((unsigned long long)0)  != 0);

	BUG_ON(variable_min_value((unsigned char)0) <
	       variable_min_value((unsigned short)0));

	BUG_ON(variable_min_value((unsigned short)0) <
	       variable_min_value((unsigned int)0));

	BUG_ON(variable_min_value((unsigned int)0) <
	       variable_min_value((unsigned long)0));

	BUG_ON(variable_min_value((unsigned long)0) <
	       variable_min_value((unsigned long long)0));

	/*  */
	BUG_ON(variable_min_value((signed char)0)  != SCHAR_MIN);
	BUG_ON(variable_min_value((signed short)0) != SHRT_MIN);
	BUG_ON(variable_min_value((signed int)0)   != INT_MIN);
	BUG_ON(variable_min_value((signed long)0)  != LONG_MIN);
	BUG_ON(variable_min_value((signed long long)0)  != LLONG_MIN);

	BUG_ON(variable_min_value((signed char)0) <
	       variable_min_value((signed short)0));

	BUG_ON(variable_min_value((signed short)0) <
	       variable_min_value((signed int)0));

	BUG_ON(variable_min_value((signed int)0) <
	       variable_min_value((signed long)0));

	BUG_ON(variable_min_value((signed long)0) <
	       variable_min_value((signed long long)0));

	/*  */

	{
		unsigned int		v;
		BUG_ON(_mul_overflow(1u, 1u, &v));
	}

	{
		unsigned char		v;

		BUG_ON(_mul_overflow((unsigned char)1u, (unsigned char)1u, &v));
		BUG_ON(_mul_overflow((unsigned char const)1u,
				     (unsigned char const)1u, &v));
		BUG_ON(!_mul_overflow((unsigned char)10u, (unsigned char)30u, &v));
	}
}
