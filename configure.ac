#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([OpenWrt-TUNet], [0.1], [pengeorge@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRNLEN
AC_CHECK_FUNCS([memset socket strndup strstr])

AC_OUTPUT
