/* Test of snprintf() function.
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#define ASSERT(expr) if (!(expr)) abort ();

int
main (int argc, char *argv[])
{
  char buf[8];
  int size;
  int retval;

  for (size = 0; size <= 8; size++)
    {
      memcpy (buf, "DEADBEEF", 8);
      retval = snprintf (buf, size, "%d", 12345);
      if (size < 6)
	{
	  ASSERT (retval < 0 || retval >= size);
	  if (size > 0)
	    {
	      ASSERT (memcmp (buf, "12345", size - 1) == 0);
	      ASSERT (buf[size - 1] == '\0' || buf[size - 1] == '0' + size);
	    }
	  ASSERT (memcmp (buf + size, "DEADBEEF" + size, 8 - size) == 0);
	}
      else
	{
	  ASSERT (retval == 5);
	  ASSERT (memcmp (buf, "12345\0EF", 8) == 0);
	}
    }

  return 0;
}