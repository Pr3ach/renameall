/*
 * This file is part of color_shell.
 * Copyright (C) 2014, Pr3acher
 * All rights reserved.
 *
 * color_shell is a free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * color_shell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with color_shell.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(COLOR_SHELL_H)
#define COLOR_SHELL_H

#define COLOR_SHELL_VERSION 1.1

void w_red(char *fmt, ...);
void w_bred(char *fmt, ...);
void w_green(char *fmt, ...);
void w_bgreen(char *fmt, ...);
void w_white(char *fmt, ...);
void w_bwhite(char *fmt, ...);
void w_blue(char *fmt, ...);
void w_bblue(char *fmt, ...);
void w_purple(char *fmt, ...);
void w_bpurple(char *fmt, ...);
void w_yellow(char *fmt, ...);
void w_byellow(char *fmt, ...);
void w_cyan(char *fmt, ...);
void w_bcyan(char *fmt, ...);
void w_rewind(void);

#if defined(_WIN32)
#include <windows.h>

void set_color(int color);
HANDLE out;

/* Color taken by SetConsoleTextAttribute() */
#define RED FOREGROUND_RED
#define GREEN FOREGROUND_GREEN
#define WHITE FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN
#define BLUE FOREGROUND_BLUE
#define PURPLE FOREGROUND_BLUE | FOREGROUND_RED
#define YELLOW FOREGROUND_RED | FOREGROUND_GREEN
#define CYAN FOREGROUND_GREEN | FOREGROUND_BLUE
#define REWIND FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN

#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__)) || defined(__unix__)
void set_color(char *color);

#define RED "\033[0;31m"         /* 31 == red */
#define RED_BOLD "\033[1;31m"    /* 1 == bold */
#define GREEN "\033[0;32m"
#define GREEN_BOLD "\033[1;32m"
#define WHITE "\033[0;37m"
#define WHITE_BOLD "\033[1;37m"
#define BLUE "\033[0;34m"
#define BLUE_BOLD "\033[1;34m"
#define PURPLE "\033[0;35m"
#define PURPLE_BOLD "\033[1;35m"
#define YELLOW "\033[0;33m"
#define YELLOW_BOLD "\033[1;33m"
#define CYAN "\033[0;36m"
#define CYAN_BOLD "\033[1;36m"
#define REWIND "\x1B[0m"

#else
#error Platform not supported by color_shell lib

#endif /* _WIN32 */
#endif /* !COLOR_SHELL_H */
