/* Test of u16_vasnprintf() function.
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

#include "unistdio.h"

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "unistr.h"

#define SIZEOF(array) (sizeof (array) / sizeof (array[0]))
#define ASSERT(expr) \
  do									     \
    {									     \
      if (!(expr))							     \
        {								     \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__); \
          abort ();							     \
        }								     \
    }									     \
  while (0)

#include "test-u16-asnprintf1.h"
#include "test-u16-printf1.h"

static uint16_t *
my_asnprintf (uint16_t *resultbuf, size_t *lengthp, const char *format, ...)
{
  va_list args;
  uint16_t *ret;

  va_start (args, format);
  ret = u16_vasnprintf (resultbuf, lengthp, format, args);
  va_end (args);
  return ret;
}

static uint16_t *
my_xasprintf (const char *format, ...)
{
  va_list args;
  size_t length;
  uint16_t *ret;

  va_start (args, format);
  ret = u16_vasnprintf (NULL, &length, format, args);
  va_end (args);
  if (ret != NULL)
    ASSERT (length == u16_strlen (ret));
  return ret;
}

static void
test_vasnprintf ()
{
  test_function (my_asnprintf);
  test_xfunction (my_xasprintf);
}

int
main (int argc, char *argv[])
{
  test_vasnprintf ();
  return 0;
}