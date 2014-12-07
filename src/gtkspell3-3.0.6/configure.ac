dnl Process this file with autoconf to produce a configure script.

m4_define(gtkspell_version_minor, 0)
m4_define(gtkspell_version_micro, 6)

AC_INIT(gtkspell3, 3.gtkspell_version_minor.gtkspell_version_micro)
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR(gtkspell/gtkspell.c)
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([-Wall])

AM_SILENT_RULES([yes])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AC_SUBST(GTK_SPELL_VERSION_MINOR, gtkspell_version_minor)
AC_SUBST(GTK_SPELL_VERSION_MICRO, gtkspell_version_micro)

dnl See http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
AC_SUBST([GTK_SPELL_SO_VERSION], [1:0:1])

AC_PROG_CC
AM_PROG_CC_C_O

dnl ************************ Enable Gtk2 GtkSpell build ***********************
AC_ARG_ENABLE([gtk2],
     AS_HELP_STRING([--enable-gtk2], [enable gtk2 build (default is no)]),
     [case "${enableval}" in
       yes) gtk2=yes ;;
       no)  gtk2=no ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-gtk2]) ;;
     esac],[gtk2=no])
AM_CONDITIONAL([ENABLE_GTK2], [test x$gtk2 = xyes])

dnl ************************ Enable Gtk3 GtkSpell build ***********************
AC_ARG_ENABLE([gtk3],
     AS_HELP_STRING([--enable-gtk3], [enable gtk3 build (default is yes)]),
     [case "${enableval}" in
       yes) gtk3=yes ;;
       no)  gtk3=no ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-gtk3]) ;;
     esac],[gtk3=yes])
AM_CONDITIONAL([ENABLE_GTK3], [test x$gtk3 = xyes])

dnl ************************** Gobject Introspection **************************
dnl See https://live.gnome.org/GObjectIntrospection/AutotoolsIntegration
GOBJECT_INTROSPECTION_CHECK([1.30.0])


dnl ****************************** Vala bindings ******************************
dnl See https://live.gnome.org/Vala/UpstreamGuide
AM_PROG_VALAC

VAPIGEN_CHECK([0.18])

if test "$enable_vala" != "no"; then
	AC_PATH_PROG([VALAC], [valac], [])
	AC_SUBST(SPEC_ENABLE_VALA, 1)
fi
AM_CONDITIONAL(HAVE_VALAC, test "x$VALAC" != "x")

dnl *************************** Internationalization **************************
IT_PROG_INTLTOOL([0.35.0])
GETTEXT_PACKAGE=$PACKAGE_NAME
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT

dnl ************************** Libtool initialization *************************
LT_INIT

dnl ********************************* Modules *********************************
if test x$gtk3 = xyes; then
	AC_SUBST([GTKSPELL3_MODULES], ['glib-2.0 gtk+-3.0 enchant'])
	PKG_CHECK_MODULES(GTK_SPELL3, [$GTKSPELL3_MODULES])
	AC_SUBST(SPEC_ENABLE_GTK3, 1)
fi

if test x$gtk2 = xyes; then
	AC_SUBST([GTKSPELL2_MODULES], ['glib-2.0 gtk+-2.0 enchant'])
	PKG_CHECK_MODULES(GTK_SPELL2, [$GTKSPELL2_MODULES])
	AC_SUBST(SPEC_ENABLE_GTK2, 1)
fi

dnl ********************************* GTK-Doc *********************************
dnl See http://developer.gnome.org/gtk-doc-manual/unstable/settingup_autoconf.html.en
m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.14],[--flavour no-tmpl])
AC_SUBST(SPEC_ENABLE_GTK_DOC, 1)
],[
AM_CONDITIONAL([ENABLE_GTK_DOC], false)
])

dnl ****************************** Detect Win32 *******************************
AC_MSG_CHECKING([for some Win32 platform])
case "$host" in
  *-*-mingw*|*-*-cygwin*)
    platform_win32=yes
    ;;
  *)
    platform_win32=no
    ;;
esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")

dnl ******************************** iso-codes ********************************
AC_ARG_ENABLE([iso-codes],
     AS_HELP_STRING([--enable-iso-codes], [enable iso-codes support (default is yes)]),
     [case "${enableval}" in
       yes) iso_codes=yes ;;
       no)  iso_codes=no ;;
       *) AC_MSG_ERROR([bad value ${enableval} for --enable-iso_codes]) ;;
     esac],[iso_codes=yes])
AM_CONDITIONAL([HAVE_ISO_CODES], [test x$iso_codes = xyes])

if test x$iso_codes = xyes; then
    iso_codes_prefix=`$PKG_CONFIG --variable=prefix iso-codes 2>/dev/null || echo /usr`
    AC_MSG_NOTICE([iso-codes prefix: $iso_codes_prefix])
    AC_DEFINE_UNQUOTED([ISO_CODES_PREFIX], ["$iso_codes_prefix"], [ISO codes prefix])
    AC_DEFINE_UNQUOTED([ISO_CODES_LOCALEDIR], ["$iso_codes_prefix/share/locale"], [ISO codes locale dir])
    AC_DEFINE([HAVE_ISO_CODES], [1], [iso-codes available])
fi

dnl ********************************* Summary *********************************
echo
echo "Configuration summary for $PACKAGE $VERSION:"
echo "  - Host:                  ${host}"
echo "  - GTK+2 support:         $gtk2"
echo "  - GTK+3 support:         $gtk3"
echo "  - GObject introspection: $enable_introspection"
echo "  - Vala bindings:         $enable_vala"
echo "  - iso-codes support:     $iso_codes"
echo "  - Documentation:         $enable_gtk_doc"
echo

AC_CONFIG_FILES([
Makefile
gtkspell3.spec
gtkspell/gtkspell3-2.0.pc
gtkspell/gtkspell3-3.0.pc
gtkspell/Makefile
examples/Makefile
docs/Makefile
docs/version.xml
po/Makefile.in])

AC_OUTPUT
