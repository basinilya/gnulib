/* Test of binary mode I/O.
   Copyright (C) 2005, 2007 Free Software Foundation, Inc.

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

/* Written by Bruno Haible <bruno@clisp.org>, 2005.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "binary-io.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ASSERT(expr) if (!(expr)) abort ();

int
main ()
{
  /* Test the O_BINARY macro.  */
  {
    int fd = open ("output2.tmp", O_CREAT | O_TRUNC | O_RDWR | O_BINARY);
    if (write (fd, "Hello\n", 6) < 0)
      exit (1);
    close (fd);
  }
  {
    struct stat statbuf;
    if (stat ("output2.tmp", &statbuf) < 0)
      exit (1);
    ASSERT (statbuf.st_size == 6);
  }
  unlink ("output2.tmp");

  /* Test the SET_BINARY macro.  */
  SET_BINARY (1);
  fputs ("Hello\n", stdout);
  fclose (stdout);
  fclose (stderr);
  {
    struct stat statbuf;
    if (stat ("output.tmp", &statbuf) < 0)
      exit (1);
    ASSERT (statbuf.st_size == 6);
  }

  return 0;
}