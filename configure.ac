AC_PREREQ([2.69])
AC_INIT([OSS Translator], [0.0], [larhard@gmail.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/oss.c])
AC_CONFIG_HEADERS([config.h])

AC_ARG_WITH([hurddir],
			[AS_HELP_STRING([--with-hurddir=DIR], [hurd translators dir])],
			[hurddir=$withval],
			[hurddir="\${exec_prefix}/hurd"])
AC_SUBST([hurddir], [$hurddir])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
