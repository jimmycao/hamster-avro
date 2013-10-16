#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([hamster-avro], [0.1], [jcao@gopivotal.com])
AC_CONFIG_SRCDIR([src/common.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(1.8 foreign -Wall -Werror)

#if we will create lib
AM_PROG_AR
AC_PROG_LIBTOOL
LT_INIT

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lavro':
AC_CHECK_LIB([avro], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT64_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
                 ])
AC_OUTPUT
