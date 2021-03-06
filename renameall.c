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
#include <unistd.h>
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
	char action = 0;
	int o = 0;
	char path[MAX_PATH] = {0};

	if (argc < 2 || strlen(argv[1]) > MAX_PATH-1)
		usage(argv[0]);

	strncpy(path, argv[1], MAX_PATH-1);

	OPTIONS = O_NONE;

	while ((o = getopt(argc, argv, "e:s:vi")) != -1)
	{
		switch((char)o)
		{
			case 'v':
				version();
				exit(0);
				break;

			case 'e':
				OPTIONS |= O_ENDSWITH;
				strncpy(ENDSWITH, optarg, 30);
				break;

			case 's':
				OPTIONS |= O_STARTSWITH;
				strncpy(STARTSWITH, optarg, 30);
				break;

			case 'i':
				OPTIONS |= O_ICASE;
				break;

			default:
				break;
		}
	}

	_calloc(&l);
	file_count = list_dir((const char *) path, l);
	w_bgreen("[+] Found %d file%s\n", file_count, file_count > 1 ? "s\n" : "\n");
	qsort(l, file_count, sizeof(char *), comp);

	for (i = 0; i < file_count; i++)
	{
		w_bwhite("'%s'", l[i]);
		w_bgreen(": ");

		if (get_newname(new_name) != 0)
			continue;

		switch (_rename(l[i], new_name, path))
		{
			case R_SUCCESS:
				break;

			case R_EXISTS:
				w_byellow("[!] File already exists. Rename, Skip (r, s) [s] ? ");
				scanf("%c", &action);

				/* clean stdin' \n */
				if (action != '\n')
					purge_stdin();

				switch(lower(action))
				{
					case 'r':
						--i;
						continue;
						break;

					case 's':
					default:
						continue;
						break;
				}
				break;

				/* rename() error; skip file */
			default:
				perror("rename");
				break;
		}
	}

	_free(l);
	exit(0);
}

void usage(const char *self)
{
	w_bwhite("Usage: %s <path> [OPTIONS]\n", self);
	w_white("  -v			Show version information\n");
	w_white("  -s<startswith>	Only show files starting with <startswith>\n");
	w_white("  -e<endswith>		Only show files ending with <endswith>\n");
	w_white("  -i			Ingore case when used with -s or -e\n");

	exit(0);
}

void version(void)
{
	w_bwhite("renameall-%.2f/%s on %s by %s\n", VERSION, PLATFORM, __DATE__, AUTHOR);
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
		w_byellow("[!] New name too long; skipping\n");
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

	if (!access(newfile, F_OK))
		return R_EXISTS;

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

	while((dir = readdir(d)))
	{
		/* Skip '.' & '..' */
		if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, ".."))
			continue;

		if ((OPTIONS & O_STARTSWITH) && (OPTIONS & O_ENDSWITH))
		{
			if (!startswith(dir->d_name, STARTSWITH, OPTIONS & O_ICASE) && !endswith(dir->d_name, ENDSWITH, OPTIONS & O_ICASE))
				strncpy(l[i++], dir->d_name, dir->d_reclen+1);
		}
		else if (OPTIONS & O_STARTSWITH)
		{
			if (!startswith(dir->d_name, STARTSWITH, OPTIONS & O_ICASE))
				strncpy(l[i++], dir->d_name, dir->d_reclen+1);
		}
		else if (OPTIONS & O_ENDSWITH)
		{
			if (!endswith(dir->d_name, ENDSWITH, OPTIONS & O_ICASE))
				strncpy(l[i++], dir->d_name, dir->d_reclen+1);
		}
		else
		{
			strncpy(l[i++], dir->d_name, dir->d_reclen+1);
		}
	}

	closedir(d);

	return i;
}

char lower(char c)
{
	if (c >= 0x41 && c <= 0x5A)
		return c + 0x20;
	return c;
}

int startswith(const char *s, const char *seq, int icase)
{
	if (icase)
		return strncasecmp(s, seq, strlen(seq));
	else
		return strncmp(s, seq, strlen(seq));
}

int endswith(const char *s, const char *seq, int icase)
{
	if (strlen(seq) > strlen(s))
		return -1;

	if (icase)
		return strcasecmp(s+strlen(s)-strlen(seq), seq);
	else
		return strcmp(s+strlen(s)-strlen(seq), seq);
}
