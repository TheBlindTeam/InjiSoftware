## Process this file with automake to produce Makefile.in

SUBDIRS = gtkspell examples po docs

ACLOCAL_AMFLAGS = -I build

INTLTOOL_FILES = intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

EXTRA_DIST = autogen.sh $(gtkspelldoc_DATA) $(INTLTOOL_FILES) po/$(GETTEXT_PACKAGE).pot

DISTCHECK_CONFIGURE_FLAGS = --enable-introspection --enable-gtk-doc
	
DISTCLEANFILES = intltool-extract \
	intltool-merge \
	intltool-update \
	po/.intltool-merge-cache


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(gtkspelldocdir)
