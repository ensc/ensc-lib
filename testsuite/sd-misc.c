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

#define xDISABLE_SYSTEMD	1

#include "../sd-listen.h"
#include "../sd-notify.h"

#include "../sd-listen.c"
#include "../sd-notify.c"
#include "../sd-notifyf.c"

#include <stdbool.h>

int main(void)
{
	sd_listen_fds(true);
	sd_notify_supported();
	sd_notify(false, "XXX");
	sd_notifyf(false, "foo=%s", "bar");
}
