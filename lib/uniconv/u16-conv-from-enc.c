/* Conversion to UTF-16 from legacy encodings.
   Copyright (C) 2002, 2006-2007 Free Software Foundation, Inc.

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

/* Written by Bruno Haible <bruno@clisp.org>.  */

#include <config.h>

/* Specification.  */
#include "uniconv.h"

#include <errno.h>
#include <stdlib.h>

#include "striconveha.h"
#include "unistr.h"

/* Name of UTF-16 encoding with machine dependent endianness and alignment.  */
#if defined _LIBICONV_VERSION || (__GLIBC__ > 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 2)
# ifdef WORDS_BIGENDIAN
#  define UTF16_NAME "UTF-16BE"
# else
#  define UTF16_NAME "UTF-16LE"
# endif
#endif

#define FUNC u16_conv_from_encoding
#define UNIT uint16_t
#define U8_TO_U u8_to_u16
#define U_MBLEN u16_mblen
#if defined UTF16_NAME
# define UTF_NAME UTF16_NAME
# define HAVE_UTF_NAME 1
#endif
#include "u-conv-from-enc.h"