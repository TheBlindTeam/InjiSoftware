noinst_PROGRAMS =

if ENABLE_GTK2
noinst_PROGRAMS += simple-gtk2 advanced-gtk2
simple_gtk2_CPPFLAGS = $(GTK_SPELL2_CFLAGS) -I$(top_srcdir)
simple_gtk2_LDADD = $(GTK_SPELL2_LIBS) $(top_builddir)/gtkspell/libgtkspell3-2.la
simple_gtk2_SOURCES = simple.c

advanced_gtk2_CPPFLAGS = $(GTK_SPELL2_CFLAGS) -I$(top_srcdir)
advanced_gtk2_LDADD = $(GTK_SPELL2_LIBS) $(top_builddir)/gtkspell/libgtkspell3-2.la
advanced_gtk2_SOURCES = advanced.c
endif

if ENABLE_GTK3
noinst_PROGRAMS += simple-gtk3 advanced-gtk3
simple_gtk3_CPPFLAGS = $(GTK_SPELL3_CFLAGS) -I$(top_srcdir)
simple_gtk3_LDADD = $(GTK_SPELL3_LIBS) $(top_builddir)/gtkspell/libgtkspell3-3.la
simple_gtk3_SOURCES = simple.c

advanced_gtk3_CPPFLAGS = $(GTK_SPELL3_CFLAGS) -I$(top_srcdir)
advanced_gtk3_LDADD = $(GTK_SPELL3_LIBS) $(top_builddir)/gtkspell/libgtkspell3-3.la
advanced_gtk3_SOURCES = advanced.c

if ENABLE_VAPIGEN
if HAVE_VALAC
noinst_PROGRAMS += simple-gtk3-vala
simple_gtk3_vala_VALAFLAGS = --vapidir $(top_builddir)/gtkspell --pkg gtkspell3-3.0 --pkg gtk+-3.0
simple_gtk3_vala_CPPFLAGS = $(GTK_SPELL3_CFLAGS) -I$(top_srcdir)
simple_gtk3_vala_LDADD = $(GTK_SPELL3_LIBS) $(top_builddir)/gtkspell/libgtkspell3-3.la
simple_gtk3_vala_SOURCES = simple-vala.vala
endif
endif
endif
