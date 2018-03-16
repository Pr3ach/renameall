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

#define VERSION 1.01
#define AUTHOR "Preacher"

#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__unix__)
#define PLATFORM "unix"
#else
#define PLATFORM "win"
#endif

#define MAX_FILES 1024
#define MAX_FILE_LENGTH 256
#define MAX_PATH 256

/* ret val for _rename() */
#define R_SUCCESS 0
#define R_EXISTS 1

/* Options mask */
#define O_NONE 0
#define O_STARTSWITH 1
#define O_ENDSWITH 2
#define O_ICASE 4

int OPTIONS;
char ENDSWITH[32];
char STARTSWITH[32];

void usage(const char *self);
void version(void);
int get_newname(char *new_name);
int comp(const void *p1, const void *p2);
void _calloc(char ***l);
void _free(char **l);
int _rename(const char *oldname, const char *newname, const char *prefix);
void purge_stdin(void);
int list_dir(const char *path, char *l[]);
char lower(char c);
int startswith(const char *s, const char *seq, int icase);
int endswith(const char *s, const char *seq, int icase);

#endif

