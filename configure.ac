
#========== Initialisation ==========

AC_INIT([wampcc], [1.4], [wampcc@darrenjs.net], [wampcc], [ ] )
AC_PREREQ([2.59])

AC_CONFIG_AUX_DIR([.])

# This macro causes the environment variables, $host, $build and $target to be
# defined.
AC_CANONICAL_SYSTEM

# Note: the -Wall here is for automake errors; is unrelated to gcc errors
AM_INIT_AUTOMAKE([1.10 no-define -Wall foreign subdir-objects] )

AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_LANG([C++])

AC_CONFIG_MACRO_DIR([m4])

# AM_PROG_AR not defined on redhat6
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# initialise libtool support
LT_INIT

#========== Checks for programs ==========

AC_PROG_CXX
AC_PROG_CC
AM_PROG_LIBTOOL
AC_PROG_INSTALL

m4_ifdef([AX_CXX_COMPILE_STDCXX_11], [
    AX_CXX_COMPILE_STDCXX_11
])


#========== Check for third party libraries ==========
#============ openssl ===========
PKG_CHECK_MODULES([OPENSSL], [openssl >= 1.0])

#============= libuv ===========
AC_ARG_WITH(libuv, [  --with-libuv=PATH specify directory containing installation of libuv])
if test "x$with_libuv" != x; then
    LIBUV_CFLAGS="-I$with_libuv/include"
    LIBUV_LIBS="-luv"
    #~ # TODO: library sanity check
    AC_MSG_NOTICE([notice libuv-root: ${with_libuv}])
else
    PKG_CHECK_MODULES([LIBUV], [libuv >= 1.8.0])
fi
AC_DEFINE_UNQUOTED([HAVE_LIBUV], [1], [Define to 1 if Jannson library is present])

#============= jansson =========
AC_ARG_WITH(jansson, [  --with-jansson=PATH specify directory for the installed JANSSON library])
if test "x$with_jansson" != x; then
    JANSSON_CFLAGS="-I$with_jansson/include"
    JANSSON_LIBS="-ljansson"
    # TODO: library sanity check
else
    PKG_CHECK_MODULES([JANSSON], [jansson >= 2.7])
fi
AC_DEFINE_UNQUOTED([HAVE_JANSSON], [1], [Define to 1 if Jannson library is present])

#========== Checks for header files ==========
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h strings.h unistd.h getopt.h])

##########################################################################
# optional support for the admin static target
##########################################################################

dnl Example of default-enabled feature
AC_MSG_CHECKING([whether to build admin_static target])
AC_ARG_ENABLE([adminstatic],
    AS_HELP_STRING([--enable-adminstatic], [Enable admin_static static target (def=no)]),
    [enable_adminstatic="$enableval"],
    [enable_adminstatic=no],)
AC_MSG_RESULT([$enable_adminstatic])
AM_CONDITIONAL([USE_ADMINSTATIC], [test x"$enable_adminstatic" = x"yes" ])

#========== Generation ==========

# List the files that will be generated. These are mainly makefiles, which
# automake will generate from the corresponding Makefile.am
AC_CONFIG_FILES([
Makefile
utility/Makefile
library/Makefile
library/json/Makefile
library/wampcc/Makefile
include/Makefile
examples/Makefile
tests/Makefile
])

# Trigger the generation of our makefiles
AC_OUTPUT
