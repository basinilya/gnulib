/* A GNU-like <math.h>.

   Copyright (C) 2002-2003, 2007 Free Software Foundation, Inc.

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

#ifndef _GL_MATH_H
#define _GL_MATH_H

#include @ABSOLUTE_MATH_H@


/* The definition of GL_LINK_WARNING is copied here.  */


#ifdef __cplusplus
extern "C" {
#endif


#if @GNULIB_MATHL@
extern long double acosl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef acosl
# define acosl(x) \
    (GL_LINK_WARNING ("acosl is unportable - "\
                      "use gnulib module mathl for portability"), \
     acosl (x))
#endif

#if @GNULIB_MATHL@
extern long double asinl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef asinl
# define asinl(x) \
    (GL_LINK_WARNING ("asinl is unportable - "\
                      "use gnulib module mathl for portability"), \
     asinl (x))
#endif

#if @GNULIB_MATHL@
extern long double atanl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef atanl
# define atanl(x) \
    (GL_LINK_WARNING ("atanl is unportable - "\
                      "use gnulib module mathl for portability"), \
     atanl (x))
#endif

#if @GNULIB_MATHL@
extern long double ceill (long double x);
#elif defined GNULIB_POSIXCHECK
# undef ceill
# define ceill(x) \
    (GL_LINK_WARNING ("ceill is unportable - "\
                      "use gnulib module mathl for portability"), \
     ceill (x))
#endif

#if @GNULIB_MATHL@
extern long double cosl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef cosl
# define cosl(x) \
    (GL_LINK_WARNING ("cosl is unportable - "\
                      "use gnulib module mathl for portability"), \
     cosl (x))
#endif

#if @GNULIB_MATHL@
extern long double expl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef expl
# define expl(x) \
    (GL_LINK_WARNING ("expl is unportable - "\
                      "use gnulib module mathl for portability"), \
     expl (x))
#endif

#if @GNULIB_MATHL@
extern long double floorl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef floorl
# define floorl(x) \
    (GL_LINK_WARNING ("floorl is unportable - "\
                      "use gnulib module mathl for portability"), \
     floorl (x))
#endif

#if @GNULIB_MATHL@
extern long double frexpl (long double x, int *exp);
#elif defined GNULIB_POSIXCHECK
# undef frexpl
# define frexpl(x,e) \
    (GL_LINK_WARNING ("frexpl is unportable - "\
                      "use gnulib module mathl for portability"), \
     frexpl (x, e))
#endif

#if @GNULIB_MATHL@
extern long double ldexpl (long double x, int exp);
#elif defined GNULIB_POSIXCHECK
# undef ldexpl
# define ldexpl(x,e) \
    (GL_LINK_WARNING ("ldexpl is unportable - "\
                      "use gnulib module mathl for portability"), \
     ldexpl (x, e))
#endif

#if @GNULIB_MATHL@
extern long double logl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef logl
# define logl(x) \
    (GL_LINK_WARNING ("logl is unportable - "\
                      "use gnulib module mathl for portability"), \
     logl (x))
#endif

#if @GNULIB_MATHL@
extern long double sinl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef sinl
# define sinl(x) \
    (GL_LINK_WARNING ("sinl is unportable - "\
                      "use gnulib module mathl for portability"), \
     sinl (x))
#endif

#if @GNULIB_MATHL@
extern long double sqrtl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef sqrtl
# define sqrtl(x) \
    (GL_LINK_WARNING ("sqrtl is unportable - "\
                      "use gnulib module mathl for portability"), \
     sqrtl (x))
#endif

#if @GNULIB_MATHL@
extern long double tanl (long double x);
#elif defined GNULIB_POSIXCHECK
# undef tanl
# define tanl(x) \
    (GL_LINK_WARNING ("tanl is unportable - "\
                      "use gnulib module mathl for portability"), \
     tanl (x))
#endif


#ifdef __cplusplus
}
#endif

#endif