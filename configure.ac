AC_INIT([ufasta], [0.0.1], [gmarcais@umd.edu])
AC_CANONICAL_HOST
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects foreign parallel-tests color-tests])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([.])
AC_CONFIG_HEADERS([config.h])

# Change default compilation flags
AC_SUBST([ALL_CXXFLAGS], [-std=c++0x])
CXXFLAGS="-std=c++0x $CXXFLAGS"
AC_LANG(C++)
AC_PROG_CXX

# Check for yaggo
AC_ARG_VAR([YAGGO], [Yaggo switch parser generator])
AS_IF([test "x$YAGGO" = "x"], [AC_PATH_PROG([YAGGO], [yaggo], [false])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
