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

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "renameall.h"
#include "color_shell/src/color_shell.h"

int main(int argc, char *argv[])
{
	int i = 0;
	int file_count = 0;
	char **l; /* l[nfile][len] */
	char new_name[MAX_FILE_LENGTH] = {0};

	if (argc < 2 || strlen(argv[1]) > MAX_PATH)
		usage(argv[0]);

	_calloc(&l);
	file_count = list_dir((const char *) argv[1], l);
	w_bgreen("[+] Found %d file%s\n", file_count, file_count > 1 ? "s\n" : "\n");
	qsort(l, file_count, sizeof(char *), comp);

	for (i = 0; i < file_count; i++)
	{
		w_bwhite("'%s'", l[i]);
		w_bgreen(": ");

		if (get_newname(new_name) != 0)
			continue;

		if (_rename(l[i], new_name, argv[1]) != 0)
		{
			perror("rename");
			_free(l);
			exit(-1);
		}
	}

	_free(l);
	exit(0);
}

void usage(const char *self)
{
	w_bwhite("Usage: %s <path>\n", self);

	exit(0);
}

/*
 * Get input
 * Return non zero == skip
 *
 * */
int get_newname(char *new_name)
{
	char *p = NULL;

	fgets(new_name, MAX_FILE_LENGTH-1, stdin);

	/* if input is empty, don't rename, just skip */
	if (new_name[0] == '\n')
		return -1;

	/* remove new line */
	if ((p = strchr(new_name, '\n')))
	{
		*p = '\0';
	}
	else
	{
		w_byellow("[-] Warning: new name too long; skipping\n");
		purge_stdin();
		return 1;
	}

	return 0;
}

int comp(const void *p1, const void *p2)
{
	return strcmp(*(char * const *)p1, *(char * const *)p2);
}

void _calloc(char ***l)
{
	int i = 0;

	*l = (char **) calloc(MAX_FILES, sizeof(char *));
	for (i = 0; i < MAX_FILES; i++)
		(*l)[i] = calloc(MAX_FILE_LENGTH, sizeof(char));
}

void _free(char **l)
{
	int i = 0;

	if (!l)
		return;

	for (i = 0; i < MAX_FILES; i++)
		free(l[i]);
	free(l);
}

int _rename(const char *oldname, const char *newname, const char *prefix)
{
	char oldfile[MAX_PATH + MAX_FILE_LENGTH + 1] = {0};
	char newfile[MAX_PATH + MAX_FILE_LENGTH + 1] = {0};

	strncat(oldfile, prefix, MAX_PATH);
	strncat(oldfile, "/", 1);
	strncat(oldfile, oldname, MAX_FILE_LENGTH);

	strncat(newfile, prefix, MAX_PATH);
	strncat(newfile, "/", 1);
	strncat(newfile, newname, MAX_FILE_LENGTH);

	return rename(oldfile, newfile);
}

/*
 * Clean stdin stream
 *
 * */
void purge_stdin(void)
{
	int c = 0;

	while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * List files in specified directory
 * Fill the list with filenames
 * Return: file count
 *
 * */
int list_dir(const char *path, char *l[])
{
	DIR *d = NULL;
	struct dirent *dir = NULL;
	int i = 0;

	if (!(d = opendir(path)))
	{
		perror("opendir");
		exit(-1);
	}

	/* Skip '.' & '..' */
	while((dir = readdir(d)))
		if (strcmp(dir->d_name, ".") && strcmp(dir->d_name, ".."))
			strncpy(l[i++], dir->d_name, dir->d_reclen+1);

	closedir(d);

	return i;
}
