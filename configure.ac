AC_INIT
AC_CONFIG_FILES([Makefile])
AC_LANG([C++])
AC_SUBST([TESTFLAGS])
AC_SUBST([NCURSES_CFLAGS])
AC_SUBST([NCURSES_LIBS])
AC_CHECK_HEADERS([time.h signal.h vector algorithm iostream], [], [exit 1])
AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="$CXXFLAGS -std=c++11"], [exit 1])
AX_CHECK_COMPILE_FLAG([-O0], [TESTFLAGS="-O0"])
AX_CHECK_COMPILE_FLAG([-fprofile-arcs], [TESTFLAGS="$TESTFLAGS -fprofile-arcs"])
AX_CHECK_COMPILE_FLAG([-ftest-coverage], [TESTFLAGS="$TESTFLAGS -ftest-coverage"])
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
PKG_CHECK_MODULES([NCURSES], [ncurses])
PKG_CHECK_MODULES([SDL], [sdl2])
AC_OUTPUT