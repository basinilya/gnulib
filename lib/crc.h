/* crc.h -- cyclic redundancy checks
   Copyright (C) 2005 Free Software Foundation, Inc.

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

/* Written by Simon Josefsson.  */

#ifndef CRC_H
# define CRC_H 1

#include <stddef.h>
#if HAVE_INTTYPES_H
# include <inttypes.h>
#endif
#if HAVE_STDINT_H
# include <stdint.h>
#endif

/* Compute CRC-32 value of LEN bytes long BUF, and return it. */
extern uint32_t crc32 (const char *buf, size_t len);

/* Incrementally update CRC-32 value CRC using LEN bytes long BUF.  In
   the first call, use 0 as the value for CRC.  Return the updated
   CRC-32 value.  */
extern uint32_t crc32_update (uint32_t crc, const char *buf, size_t len);

/* Compute modified-CRC-32 value of LEN bytes long BUF, and return it.
   The "modification" is to avoid the initial and final XOR operation.
   Due to historic implementation errors, this variant is sometimes
   used (i.e., in RFC 3961). */
extern uint32_t crc32_no_xor (const char *buf, size_t len);

/* Incrementally update modified-CRC-32 value CRC using LEN bytes long
   BUF.  In the first call, use 0 as the value for CRC.  Return the
   updated modified-CRC-32 value.  The "modification" is to avoid the
   initial and final XOR operation.  Due to historic implementation
   errors, this variant is sometimes used (i.e., in RFC 3961).  */
extern uint32_t
crc32_update_no_xor (uint32_t crc, const char *buf, size_t len);

#endif /* CRC_H */