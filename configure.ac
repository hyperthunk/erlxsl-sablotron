#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AM_INIT_AUTOMAKE([-Wall -Werror -fPIC foreign])
AC_INIT([erlxsl_sablot],[0.1])
# AC_INIT([FULL-PACKAGE-NAME],[VERSION],[BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/erlxsl_sablot.c])
AC_CONFIG_HEADER([include/config.h])
AC_ARG_WITH([erlxsl-include-path],
  [AS_HELP_STRING([--with-erlxsl-include-path],
    [location of the erlxsl headers, defaults to /usr/include/erlxsl])],
  [ERLXSL_CFLAGS="-I$withval"],
  [ERLXSL_CFLAGS='-I/usr/include/erlxsl'])
AC_SUBST([ERLXSL_CFLAGS])
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
