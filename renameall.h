/*
 *  Copyright (C) 2018 - Preacher
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RENAMEALL_H
#define RENAMEALL_H

#define MAX_FILES 1024
#define MAX_FILE_LENGTH 256
#define MAX_PATH 256

#define R_SUCCESS 0
#define R_EXISTS 1

void usage(const char *self);
int get_newname(char *new_name);
int comp(const void *p1, const void *p2);
void _calloc(char ***l);
void _free(char **l);
int _rename(const char *oldname, const char *newname, const char *prefix);
void purge_stdin(void);
int list_dir(const char *path, char *l[]);
char lower(char c);

#endif

