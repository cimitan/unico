noinst_HEADERS = \
	unico.h \
	unico-cairo-support.h \
	unico-css-support.h \
	unico-draw.h \
	unico-engine.h \
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
unico_LTLIBRARIES = libunico.la

libunico_la_SOURCES = $(source_c)

libunico_la_CFLAGS = $(UNICO_CFLAGS)

libunico_la_LIBADD = $(UNICO_LIBADD)

libunico_la_LDFLAGS = $(UNICO_LDFLAGS)

libunico_includedir = \
	$(includedir)/unico-@UNICO_MAJORMINOR@/unico

libunico_include_HEADERS = \
	$(source_h) \
	unico-version.h

EXTRA_DIST = \
	unico-version.h.in

DISTCLEANFILES = \
	unico-version.h
