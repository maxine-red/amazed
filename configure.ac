AC_INIT
AC_CONFIG_FILES([Makefile])
AC_LANG([C++])
AC_PROG_CXX
AC_SUBST([TESTFLAGS])
AC_SUBST([NCURSES_CFLAGS])
AC_SUBST([NCURSES_LIBS])
AC_CHECK_HEADERS([time.h signal.h math.h vector algorithm thread chrono mutex iostream], [], [exit 1])
AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="$CXXFLAGS -std=c++11"], [exit 1])
AX_CHECK_COMPILE_FLAG([-O0], [TESTFLAGS="-O0"])
AX_CHECK_COMPILE_FLAG([-fprofile-arcs], [TESTFLAGS="$TESTFLAGS -fprofile-arcs"])
AX_CHECK_COMPILE_FLAG([-ftest-coverage], [TESTFLAGS="$TESTFLAGS -ftest-coverage"])
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
AM_CONDITIONAL([CHECK_WINDOWS], [test "$host" = "x86_64-w64-mingw32" -o "$host" = "i686-w64-mingw32"])
AM_CONDITIONAL([CHECK_32WINDOWS], [test "$host" = "i686-w64-mingw32"])
AM_CONDITIONAL([CHECK_64WINDOWS], [test "$host" = "x86_64-w64-mingw32"])
AM_CONDITIONAL([CHECK_I686], [test "$host" = "i686-linux-gnu"])
AM_COND_IF([CHECK_WINDOWS], [CPPFLAGS="$CPPFLAGS -DWINDOWS -I."
            LDFLAGS="$LDFLAGS -static -L."], [PKG_CHECK_MODULES([NCURSES],
                                                                  [ncurses])
            LIBS="$LIBS -lpthread"])
AM_COND_IF([CHECK_32WINDOWS], [LIBS="$LIBS -lwinpdcurses_32"], [])
AM_COND_IF([CHECK_64WINDOWS], [LIBS="$LIBS -lwinpdcurses"], [])
AM_COND_IF([CHECK_I686], [CXXFLAGS="$CXXFLAGS -m32"], [])
AC_OUTPUT
