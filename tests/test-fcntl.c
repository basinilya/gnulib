/* Test of <fcntl.h> substitute.
   Copyright (C) 2007 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by Bruno Haible <bruno@clisp.org>, 2007.  */

#include <config.h>

#include <fcntl.h>

/* Check that the various O_* macros are defined.  */
int o = O_DIRECT | O_DIRECTORY | O_DSYNC | O_NDELAY | O_NOATIME | O_NONBLOCK
	| O_NOCTTY | O_NOFOLLOW | O_NOLINKS | O_RSYNC | O_SYNC
	| O_BINARY | O_TEXT;

int
main ()
{
  return 0;
}