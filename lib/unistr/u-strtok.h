/* Tokenize UTF-8/UTF-16/UTF-32 string.
   Copyright (C) 1999, 2002, 2006 Free Software Foundation, Inc.
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

UNIT *
FUNC (UNIT *str, const UNIT *delim, UNIT **ptr)
{
  if (str == NULL)
    {
      str = *ptr;
      if (str == NULL)
	return NULL; /* reminder that end of token sequence has been reached */
    }

  /* Skip leading delimiters.  */
  str += U_STRSPN (str, delim);

  /* Found a token?  */
  if (*str == 0)
    {
      *ptr = NULL;
      return NULL;
    }

  /* Move past the token. */
  {
    UNIT *token_end = U_STRPBRK (str, delim);
    if (token_end)
      {
	/* NUL-terminate the token.  */
	*token_end = 0;
	*ptr = token_end + 1;
      }
    else
      *ptr = NULL;
  }

  return str;
}