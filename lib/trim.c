/* Removes leading and/or trailing whitespaces
   Copyright (C) 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.
   If not, write to the Free Software Foundation,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by Davide Angelocola <davide.angelocola@gmail.com> */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#if HAVE_MBRTOWC 
# include <stddef.h>
# include "mbchar.h"
# include "mbiter.h"
# include "mbuiter.h"		/* FIXME: for MB_CUR_MAX */
#else
# include <ctype.h>
#endif

#include "xalloc.h"
#include "trim.h"

char *
trim2(const char *s, int how)
{
  char *d;
    
  d = strdup(s);

  if (!d)
    xalloc_die();
  
#if HAVE_MBRTOWC
  if (MB_CUR_MAX > 1)
    {
      mbi_iterator_t i;
      
      /* Trim leading whitespaces. */
      if (how != TRIM_TRAILING) 
	{
	  mbi_init (i, d, strlen (d));
      
	  for (; mbi_avail (i) && mb_isspace (mbi_cur (i)); mbi_advance (i))
	    ;
	  
	  memmove (d, mbi_cur_ptr (i), strlen (mbi_cur_ptr (i)) + 1);
	}
  
      /* Trim trailing whitespaces. */
      if (how != TRIM_LEADING) 
	{
	  int state = 0;
	  char *r;
	  
	  mbi_init (i, d, strlen (d));

	  for (; mbi_avail (i); mbi_advance (i)) 
	    {
	      if (state == 0 && mb_isspace (mbi_cur (i))) 
		{
		  state = 0;
		  continue;
		}
	      
	      if (state == 0 && !mb_isspace (mbi_cur (i)))
		{
		  state = 1;
		  continue;
		}
	      
	      if (state == 1 && !mb_isspace (mbi_cur (i)))
		{
		  state = 1;
		  continue;
		}
	      
	      if (state == 1 && mb_isspace (mbi_cur (i))) 
		{
		  state = 2;
		  r = (char *) mbi_cur_ptr (i);
		}
	      else if (state == 2 && mb_isspace (mbi_cur (i))) 
		{
		  state = 2;
		} 
	      else 
		{
		  state = 1;
		}
	    }
	  
	  if (state == 2) 
	    *r = '\0';
	}
    }
  else
#endif /* HAVE_MBRTOWC */
    {
      char *p;
      
      /* Trim leading whitespaces. */
      if (how != TRIM_TRAILING) {
	for (p = d; *p && isspace (*p); p++)
	  ;			

	memmove (d, p, strlen (p) + 1);
      }

      /* Trim trailing whitespaces. */
      if (how != TRIM_LEADING) {
	for (p = d + strlen (d) - 1; p >= d && isspace (*p); p--)
	  *p = '\0';
      }
    }
  
  return d;
}
  