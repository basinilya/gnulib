/* Formatted output to strings.
   Copyright (C) 1999, 2002, 2006-2007 Free Software Foundation, Inc.

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
#include "unistdio.h"

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include "unistr.h"

#define VSNPRINTF u32_vsnprintf
#define VASNPRINTF u32_vasnprintf
#define FCHAR_T char
#define DCHAR_T uint32_t
#define DCHAR_CPY u32_cpy
#include "u-vsnprintf.h"