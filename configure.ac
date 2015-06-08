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

# AC_CHECK_LIB(boost_regex, toto)
AC_MSG_CHECKING([for libboost_regex])
AC_LINK_IFELSE([AC_LANG_PROGRAM([#include <boost/regex.hpp>],
                                [boost::regex regexp])],
                                [AC_MSG_RESULT([yes])] [AC_DEFINE([HAVE_BOOST_REGEX], 1, [Have libboost_regex])]
                                                       [have_boost=true],
                                [AC_MSG_RESULT([no])]
                                [have_boost=false])
AM_CONDITIONAL([BOOST_REGEX], [test x$have_boost = xtrue])

# Check for yaggo
AC_ARG_VAR([YAGGO], [Yaggo switch parser generator])
AS_IF([test "x$YAGGO" = "x"], [AC_PATH_PROG([YAGGO], [yaggo], [false])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
