# mbsrchr.m4 serial 1
dnl Copyright (C) 2007 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_MBSRCHR],
[
  gl_PREREQ_MBSRCHR
])

# Prerequisites of lib/mbsrchr.c.
AC_DEFUN([gl_PREREQ_MBSRCHR], [
  AC_REQUIRE([gl_FUNC_MBRTOWC])
  :
])
