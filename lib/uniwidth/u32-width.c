/* Determine display width of UTF-32 string.
   Copyright (C) 2001-2002, 2006 Free Software Foundation, Inc.
   Written by Bruno Haible <bruno@clisp.org>, 2002.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
   USA.  */

#include <config.h>

/* Specification.  */
#include "uniwidth.h"

int
u32_width (const uint32_t *s, size_t n, const char *encoding)
{
  const uint32_t *s_end = s + n;
  int width = 0;

  while (s < s_end)
    {
      ucs4_t uc = *s++;
      int w;

      if (uc == 0)
	break; /* end of string reached */

      w = uc_width (uc, encoding);
      if (w >= 0) /* ignore control characters in the string */
	width += w;
    }

  return width;
}