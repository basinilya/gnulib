/* Count characters in UTF-16 string.
   Copyright (C) 2007 Free Software Foundation, Inc.
   Written by Bruno Haible <bruno@clisp.org>, 2007.

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
#include "unistr.h"

size_t
u16_mbsnlen (const uint16_t *s, size_t n)
{
  size_t characters;

  characters = 0;
  while (n > 0)
    {
      int count = u16_mblen (s, n);
      if (count <= 0)
	count = 1;
      s += count;
      n -= count;
      characters++;
    }
  return characters;
}