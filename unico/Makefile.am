INCLUDES = $(GTK_CFLAGS)

enginedir = $(libdir)/gtk-3.0/$(GTK_VERSION)/theming-engines

engine_LTLIBRARIES = libunico.la

libunico_la_SOURCES = \
	unico.h \
	unico-draw.h \
	unico-draw.c \
	unico-engine.h \
	unico-engine.c \
	unico-theme.c \
	unico-types.h

libunico_la_LDFLAGS = -module -avoid-version

libunico_la_LIBADD = $(NULL_LIBS)
