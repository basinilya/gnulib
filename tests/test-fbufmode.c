/* Test of fbufmode() function.
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

#include "fbufmode.h"

#include <stdlib.h>

#define ASSERT(expr) if (!(expr)) abort ();

#define TESTFILE "t-fbufmode.tmp"

int
main ()
{
  FILE *fp;
  char buf[5];

  /* Create a file with some contents.  */
  fp = fopen (TESTFILE, "w");
  if (fp == NULL)
    goto skip;
  if (fwrite ("foobarsh", 1, 8, fp) < 8)
    goto skip;
  if (fclose (fp))
    goto skip;

  /* Open it for reading.  */
  fp = fopen (TESTFILE, "r");

  if (setvbuf (fp, NULL, _IONBF, 0))
    goto skip;
  ASSERT (fbufmode (fp) == _IONBF);

  /* This setvbuf call can fail, e.g. on HP-UX 11.  */
  if (setvbuf (fp, buf, _IOLBF, 5) == 0)
    {
      ASSERT (fbufmode (fp) == _IOLBF);
    }

  /* This setvbuf call can fail, e.g. on HP-UX 11.  */
  if (setvbuf (fp, buf, _IOFBF, 5) == 0)
    {
      ASSERT (fbufmode (fp) == _IOFBF);
    }

  fclose (fp);

  return 0;

 skip:
  fprintf (stderr, "Skipping test: file operations failed.\n");
  return 77;
}