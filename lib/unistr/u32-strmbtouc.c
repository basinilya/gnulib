/* Look at first character in UTF-32 string.
   Copyright (C) 2002, 2006-2007 Free Software Foundation, Inc.
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
#include "unistr.h"

int
u32_strmbtouc (ucs4_t *puc, const uint32_t *s)
{
  uint32_t c = *s;

#if CONFIG_UNICODE_SAFETY
  if (c < 0xd800 || (c >= 0xe000 && c < 0x110000))
#endif
    {
      *puc = c;
      return (c != 0 ? 1 : 0);
    }
#if CONFIG_UNICODE_SAFETY
  /* invalid multibyte character */
  return -1;
#endif
}