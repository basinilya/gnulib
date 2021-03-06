# imaxdiv.m4 serial 1
dnl Copyright (C) 2006 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_IMAXDIV],
[
  AC_REQUIRE([gl_INTTYPES_H])
  if test "$ac_cv_have_decl_imaxdiv" != yes; then
    AC_LIBOBJ([imaxdiv])
    gl_PREREQ_IMAXDIV
  fi
])

# Prerequisites of lib/imaxdiv.c.
AC_DEFUN([gl_PREREQ_IMAXDIV], [:])
