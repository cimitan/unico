AC_INIT([unico], [2.91.0])
AC_CONFIG_SRCDIR([README])

AM_INIT_AUTOMAKE([1.9 no-dist-gzip dist-bzip2 -Wall])

# Use AM_SILENT_RULES if present
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Initialize maintainer mode
AM_MAINTAINER_MODE

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_CC_C_O
AM_PATH_GLIB_2_0

dnl Initialize libtool
AM_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

dnl Initialize intltool
IT_PROG_INTLTOOL([0.40.0])

GETTEXT_PACKAGE=unico
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],["$GETTEXT_PACKAGE"],[Gettext package])
AM_GLIB_GNU_GETTEXT

# Don't forget to update the README when changing this.
PKG_CHECK_MODULES(GTK, gtk+-3.0 >= 2.91.6,,
                  AC_MSG_ERROR([GTK+-3.0 is required to compile unico]))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

GTK_VERSION=`$PKG_CONFIG --variable=gtk_binary_version gtk+-3.0`
AC_SUBST(GTK_VERSION)

AC_SUBST(BUILD_ENGINES)

AM_CONFIG_HEADER([config.h])

AC_CONFIG_FILES([
Makefile
po/Makefile
unico/Makefile
])

AC_OUTPUT

echo "
        unico $VERSION
        ====================================

        prefix:                   ${prefix}
        compiler:                 ${CC}
        cflags:                   ${CFLAGS}
"
