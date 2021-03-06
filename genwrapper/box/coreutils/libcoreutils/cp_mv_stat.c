/* vi: set sw=4 ts=4: */
/*
 * coreutils utility routine
 *
 * Copyright (C) 2003  Manuel Novoa III  <mjn3@codepoet.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "libbb.h"
#include "coreutils.h"

int cp_mv_stat2(const char *fn, struct stat *fn_stat, stat_func sf)
{

	char* fn_stripped;
	int fn_last;

	// atisu: On windows moving to a destination name ending with '/' will make 
	//        it fail ('permission denied'). Strip all ending '/' -s, but do not
	//        touch the last one if the root dir ('/') is the target.
	fn_stripped = strdup(fn);
	fn_last = strlen(fn_stripped);
	while (fn_stripped[--fn_last] == '/' && fn_last)
	        fn_stripped[fn_last] = '\0';
	if (sf(fn_stripped, fn_stat) < 0) {
		if (errno != ENOENT) {
#if ENABLE_FEATURE_VERBOSE_CP_MESSAGE
			if (errno == ENOTDIR) {
				bb_error_msg("cannot stat '%s': Path has non-directory component", fn_stripped);
				free(fn_stripped);
				return -1;
			}
#endif
			bb_perror_msg("cannot stat '%s'", fn_stripped);
			free(fn_stripped);
			return -1;
		}
		free(fn_stripped);
		return 0;
	}
	free(fn_stripped);
	if (S_ISDIR(fn_stat->st_mode)) {
		return 3;
	}
	return 1;
}

int cp_mv_stat(const char *fn, struct stat *fn_stat)
{
	return cp_mv_stat2(fn, fn_stat, stat);
}
