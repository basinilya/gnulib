# printf-frexpl.m4 serial 1
dnl Copyright (C) 2007 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl Check how to define printf_frexpl() without linking with libm.

AC_DEFUN([gl_FUNC_PRINTF_FREXPL],
[
  AC_REQUIRE([gt_TYPE_LONGDOUBLE])
  if test $gt_cv_c_long_double = yes; then
    AC_CACHE_CHECK([whether frexpl can be used without linking with libm],
      [gl_cv_func_frexpl_no_libm],
      [
        AC_TRY_LINK([#include <math.h>
                     long double x;
                     int y;],
                    [return frexpl (x, &y) < 1;],
          [gl_cv_func_frexpl_no_libm=yes],
          [gl_cv_func_frexpl_no_libm=no])
      ])
    if test $gl_cv_func_frexpl_no_libm = yes; then
      AC_DEFINE([HAVE_FREXPL_IN_LIBC], 1,
        [Define if the frexpl function is available in libc.])
    fi

    AC_CACHE_CHECK([whether ldexpl can be used without linking with libm],
      [gl_cv_func_ldexpl_no_libm],
      [
        AC_TRY_LINK([#include <math.h>
                     long double x;
                     int y;],
                    [return ldexpl (x, y) < 1;],
          [gl_cv_func_ldexpl_no_libm=yes],
          [gl_cv_func_ldexpl_no_libm=no])
      ])
    if test $gl_cv_func_ldexpl_no_libm = yes; then
      AC_DEFINE([HAVE_LDEXPL_IN_LIBC], 1,
        [Define if the ldexpl function is available in libc.])
    fi
  fi
])