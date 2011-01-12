INCLUDES = $(GTK_CFLAGS)

enginedir = $(libdir)/gtk-3.0/$(GTK_VERSION)/theming-engines

engine_LTLIBRARIES = libunico.la

libunico_la_SOURCES = \
	unico.h \
	unico-cairo-support.h \
	unico-cairo-support.c \
	unico-css-support.h \
	unico-css-support.c \
	unico-draw.h \
	unico-draw.c \
	unico-engine.h \
	unico-engine.c \
	unico-support.c \
	unico-support.h \
	unico-theme.c \
	unico-types.h

libunico_la_LDFLAGS = -module -avoid-version

libunico_la_LIBADD = $(NULL_LIBS)
