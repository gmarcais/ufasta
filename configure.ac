AC_INIT([ufasta], [0.0.3], [gmarcais@umd.edu])
AC_CANONICAL_HOST
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([subdir-objects foreign parallel-tests color-tests])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([.])
AC_CONFIG_HEADERS([config.h])

# Change default compilation flags
AC_SUBST([ALL_CXXFLAGS], [-std=c++0x])
AC_LANG(C++)
AC_PROG_CXX
CXXFLAGS="-std=c++0x $CXXFLAGS"

# Check for mremap
AC_CHECK_FUNCS([mremap])

# AC_CHECK_LIB(boost_regex, toto)
AC_ARG_VAR([BOOST_ROOT], [Root of boost installation])
AS_IF([test x$BOOST_ROOT != x],
      [CXXFLAGS="$CXXFLAGS -I$BOOST_ROOT"])
AC_MSG_CHECKING([for libboost_regex])
SAVE_LDFLAGS=$LDFLAGS
LDFLAGS="$LDFLAGS -lboost_regex"
AC_LINK_IFELSE([AC_LANG_PROGRAM([#include <boost/regex.hpp>],
                                [boost::regex exp(".*"); boost::regex_match("foo", exp);])],
                                [AC_MSG_RESULT([yes])]
                                [AC_DEFINE([HAVE_BOOST_REGEX], 1, [Have libboost_regex])]
                                [AC_SUBST([BOOST_REGEX], [true])]
                                [have_boost=true],
                                [AC_MSG_RESULT([no])]
                                [AC_SUBST([BOOST_REGEX], [])]
                                [have_boost=false])
LDFLAGS=$SAVE_LDFLAGS
AM_CONDITIONAL([HAVE_BOOST_REGEX], [test x$have_boost = xtrue])

# Check for yaggo
AC_ARG_VAR([YAGGO], [Yaggo switch parser generator])
AS_IF([test "x$YAGGO" = "x"], [AC_PATH_PROG([YAGGO], [yaggo], [false])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
