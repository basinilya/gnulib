# round.m4 serial 3
dnl Copyright (C) 2007 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_ROUND],
[
  AC_REQUIRE([gl_MATH_H_DEFAULTS])
  dnl Persuade glibc <math.h> to declare round().
  AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])
  AC_CHECK_DECLS([round], , , [#include <math.h>])
  if test "$ac_cv_have_decl_round" = yes; then
    gl_CHECK_MATH_LIB([ROUND_LIBM], [x = round (x);])
  fi
  if test "$ac_cv_have_decl_round" != yes || test "$ROUND_LIBM" = missing; then
    gl_CHECK_MATH_LIB([ROUND_LIBM], [x = floor (x) + ceil (x);])
    HAVE_DECL_ROUND=0
    AC_LIBOBJ([round])
  fi
  AC_SUBST([HAVE_DECL_ROUND])
  AC_SUBST([ROUND_LIBM])])