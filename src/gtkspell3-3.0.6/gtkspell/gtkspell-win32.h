/* gtkspell - a spell-checking addon for GTK's TextView widget
 * Copyright (c) 2013 Sandro Mani
 *
 * Based on gtkhtml-editor-spell-language.c code which is
 * Copyright (C) 2008 Novell, Inc.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* vim: set ts=4 sw=4 wm=5 : */

#ifndef GTK_SPELL_WIN32_H
#define GTK_SPELL_WIN32_H

#include "../config.h"
#include <glib.h>

G_BEGIN_DECLS

const gchar *get_win32_prefix    (void);
const gchar *get_win32_localedir (void);

G_END_DECLS

#undef PACKAGE_LOCALE_DIR
#define PACKAGE_LOCALE_DIR get_win32_localedir ()

#ifdef HAVE_ISO_CODES

#undef ISO_CODES_PREFIX
#define ISO_CODES_PREFIX get_win32_prefix ()

#undef ISO_CODES_LOCALEDIR
#define ISO_CODES_LOCALEDIR get_win32_localedir ()

#endif /* HAVE_ISO_CODES */

#endif /* GTK_SPELL_WIN32_H */
