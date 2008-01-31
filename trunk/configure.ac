#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT(Gnoll, 0.1.3, gnoll-devel@gna.org)
AC_CONFIG_SRCDIR([src/main.cpp])

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([src/config.h])
AM_INIT_AUTOMAKE([dist-bzip2])

# Checks for programs.
AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lOIS':

AC_CHECK_FUNCS([gettimeofday]) 
AC_CHECK_HEADERS([stddef.h]) 
AC_CHECK_HEADERS([sys/time.h])
AC_HEADER_TIME 


AC_CHECK_LIB([OIS], [main])

PKG_CHECK_MODULES(OGRE, [OGRE >= 1.4])
AC_SUBST(OGRE_CFLAGS)
AC_SUBST(OGRE_LIBS)

PKG_CHECK_MODULES(CEGUI-OGRE, [CEGUI-OGRE >= 0.5])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
