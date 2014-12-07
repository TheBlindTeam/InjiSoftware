## Process this file with automake to produce Makefile.in

lib_LTLIBRARIES =
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA =

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

if ENABLE_GTK3
lib_LTLIBRARIES += libgtkspell3-3.la
libgtkspell3_3_la_CPPFLAGS = -DPACKAGE_LOCALE_DIR=\""$(localedir)"\" $(GTK_SPELL3_CFLAGS)
libgtkspell3_3_la_includedir=$(includedir)/gtkspell-3.0/gtkspell
libgtkspell3_3_la_include_HEADERS = gtkspell.h

libgtkspell3_3_la_SOURCES = gtkspell.c gtkspell.h
if HAVE_ISO_CODES
libgtkspell3_3_la_SOURCES += gtkspell-codetable.c gtkspell-codetable.h
endif
if PLATFORM_WIN32
libgtkspell3_3_la_SOURCES += gtkspell-win32.c gtkspell-win32.h
endif
libgtkspell3_3_la_LIBADD = $(GTK_SPELL3_LIBS)
libgtkspell3_3_la_LDFLAGS = -version-info $(GTK_SPELL_SO_VERSION) $(no_undefined)

pkgconfig_DATA += gtkspell3-3.0.pc
endif

if ENABLE_GTK2
lib_LTLIBRARIES += libgtkspell3-2.la
libgtkspell3_2_la_CPPFLAGS = -DPACKAGE_LOCALE_DIR=\""$(localedir)"\" $(GTK_SPELL2_CFLAGS)
libgtkspell3_2_la_includedir=$(includedir)/gtkspell-3.0/gtkspell
libgtkspell3_2_la_include_HEADERS = gtkspell.h

libgtkspell3_2_la_SOURCES = gtkspell.c gtkspell.h
if HAVE_ISO_CODES
libgtkspell3_2_la_SOURCES += gtkspell-codetable.c gtkspell-codetable.h
endif
if PLATFORM_WIN32
libgtkspell3_2_la_SOURCES += gtkspell-win32.c gtkspell-win32.h
endif
libgtkspell3_2_la_LIBADD = $(GTK_SPELL2_LIBS)
libgtkspell3_2_la_LDFLAGS = -version-info $(GTK_SPELL_SO_VERSION) $(no_undefined)

pkgconfig_DATA += gtkspell3-2.0.pc
endif

EXTRA_DIST = $(pkgconfig_DATA)

include $(INTROSPECTION_MAKEFILE)

if HAVE_INTROSPECTION
INTROSPECTION_GIRS =

if ENABLE_GTK3
GtkSpell-3.0.gir: libgtkspell3-3.la
GtkSpell_3_0_gir_INCLUDES = GObject-2.0 GLib-2.0 Gtk-3.0
GtkSpell_3_0_gir_CFLAGS = $(GTK_SPELL3_CFLAGS)
GtkSpell_3_0_gir_LIBS = libgtkspell3-3.la
GtkSpell_3_0_gir_FILES = gtkspell.c gtkspell.h
GtkSpell_3_0_gir_SCANNERFLAGS = --warn-all --c-include gtkspell/gtkspell.h
INTROSPECTION_GIRS += GtkSpell-3.0.gir
endif

if ENABLE_GTK2
GtkSpell-2.0.gir: libgtkspell3-2.la
GtkSpell_2_0_gir_INCLUDES = GObject-2.0 GLib-2.0 Gtk-2.0
GtkSpell_2_0_gir_CFLAGS = $(GTK_SPELL2_CFLAGS)
GtkSpell_2_0_gir_LIBS = libgtkspell3-2.la
GtkSpell_2_0_gir_FILES = gtkspell.c gtkspell.h
GtkSpell_2_0_gir_SCANNERFLAGS = --warn-all --c-include gtkspell/gtkspell.h
INTROSPECTION_GIRS += GtkSpell-2.0.gir
endif

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(libdir)/girepository-1.0
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES = $(gir_DATA) $(typelib_DATA)

if ENABLE_VAPIGEN
include $(VAPIGEN_MAKEFILE)

VAPIGEN_VAPIS =

if ENABLE_GTK3
gtkspell3-3.0.vapi: GtkSpell-3.0.gir gtkspell3-3.0.deps
gtkspell3_3_0_vapi_DEPS = gtk+-3.0
gtkspell3_3_0_vapi_FILES = GtkSpell-3.0.gir

gtkspell3-3.0.deps:
	$(AM_V_GEN) for pkg in $(gtkspell3_3_0_vapi_DEPS); do \
		echo $$pkg >> $@; \
	done

VAPIGEN_VAPIS += gtkspell3-3.0.vapi
endif

if ENABLE_GTK2
gtkspell3-2.0.vapi: GtkSpell-2.0.gir gtkspell3-2.0.deps
gtkspell3_2_0_vapi_DEPS = gtk+-2.0
gtkspell3_2_0_vapi_FILES = GtkSpell-2.0.gir

gtkspell3-2.0.deps:
	$(AM_V_GEN) for pkg in $(gtkspell3_2_0_vapi_DEPS); do \
		echo $$pkg >> $@; \
	done

VAPIGEN_VAPIS += gtkspell3-2.0.vapi
endif

vapidir = $(datadir)/vala/vapi
vapi_DATA = $(VAPIGEN_VAPIS) $(VAPIGEN_VAPIS:.vapi=.deps)

CLEANFILES += $(vapi_DATA)
endif
endif
