noinst_HEADERS = \
	unico.h \
	unico-cairo-support.h \
	unico-css-support.h \
	unico-draw.h \
	unico-engine.h \
	unico-support.c \
	unico-types.h

source_h =

source_c = \
	$(srcdir)/unico-cairo-support.c \
	$(srcdir)/unico-css-support.c \
	$(srcdir)/unico-draw.c \
	$(srcdir)/unico-engine.c \
	$(srcdir)/unico-support.c \
	$(srcdir)/unico-theme.c

unicoincludedir = $(includedir)/unico-@UNICO_MAJORMINOR@/unico
unicoinclude_HEADERS = $(source_h)

unicodir = $(libdir)/gtk-3.0/$(GTK_VERSION)/theming-engines
unico_LTLIBRARIES = libunico-@UNICO_MAJORMINOR@.la

libunico_@UNICO_MAJORMINOR@_la_SOURCES = $(source_c)

libunico_@UNICO_MAJORMINOR@_la_CFLAGS = $(UNICO_CFLAGS)

libunico_@UNICO_MAJORMINOR@_la_LIBADD = $(UNICO_LIBADD)

libunico_@UNICO_MAJORMINOR@_la_LDFLAGS = $(UNICO_LDFLAGS) -module -avoid-version

libunico_@UNICO_MAJORMINOR@_includedir = \
	$(includedir)/unico-@UNICO_MAJORMINOR@/unico

libunico_@UNICO_MAJORMINOR@_include_HEADERS = \
	$(source_h) \
	unico-version.h

EXTRA_DIST = \
	unico-version.h.in

DISTCLEANFILES = \
	unico-version.h
