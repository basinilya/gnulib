# snprintf-posix.m4 serial 1
dnl Copyright (C) 2007 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

AC_DEFUN([gl_FUNC_SNPRINTF_POSIX],
[
  AC_REQUIRE([gl_EOVERFLOW])
  AC_REQUIRE([gl_PRINTF_SIZES_C99])
  AC_REQUIRE([gl_PRINTF_DIRECTIVE_A])
  AC_REQUIRE([gl_PRINTF_DIRECTIVE_N])
  AC_REQUIRE([gl_PRINTF_POSITIONS])
  gl_cv_func_snprintf_posix=no
  AC_CHECK_FUNCS([snprintf])
  if test $ac_cv_func_snprintf = yes; then
    gl_SNPRINTF_TRUNCATION_C99
    gl_SNPRINTF_RETVAL_C99
    gl_SNPRINTF_DIRECTIVE_N
    case "$gl_cv_func_printf_sizes_c99" in
      *yes)
        case "$gl_cv_func_printf_directive_a" in
          *yes)
            case "$gl_cv_func_printf_directive_n" in
              *yes)
                case "$gl_cv_func_printf_positions" in
                  *yes)
                    case "$gl_cv_func_snprintf_truncation_c99" in
                      *yes)
                        case "$gl_cv_func_snprintf_retval_c99" in
                          *yes)
                            case "$gl_cv_func_snprintf_directive_n" in
                              *yes)
                                # snprintf exists and is already POSIX
                                # compliant.
                                gl_cv_func_snprintf_posix=yes
                                ;;
                            esac
                            ;;
                        esac
                        ;;
                    esac
                    ;;
                esac
                ;;
            esac
            ;;
        esac
        ;;
    esac
  fi
  if test $gl_cv_func_snprintf_posix = no; then
    if ! expr "$gl_cv_func_printf_directive_a" : ".*yes" > /dev/null; then
      AC_DEFINE([NEED_PRINTF_DIRECTIVE_A], 1,
        [Define if the vasnprintf implementation needs special code for
         the 'a' and 'A' directives.])
    fi
    gl_REPLACE_VASNPRINTF
    gl_REPLACE_SNPRINTF
  fi
])