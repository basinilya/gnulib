/* Look at first character in UTF-16 string.
   Copyright (C) 1999-2000, 2002, 2006-2007 Free Software Foundation, Inc.
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
u16_strmbtouc (ucs4_t *puc, const uint16_t *s)
{
  /* Keep in sync with utf16-ucs4.h and utf16-ucs4.c.  */
  uint16_t c = *s;

  if (c < 0xd800 || c >= 0xe000)
    {
      *puc = c;
      return (c != 0 ? 1 : 0);
    }
#if CONFIG_UNICODE_SAFETY
  if (c < 0xdc00)
#endif
    {
#if CONFIG_UNICODE_SAFETY
      if (s[1] >= 0xdc00 && s[1] < 0xe000)
#else
      if (s[1] != 0)
#endif
	{
	  *puc = 0x10000 + ((c - 0xd800) << 10) + (s[1] - 0xdc00);
	  return 2;
	}
    }
  /* invalid or incomplete multibyte character */
  return -1;
}