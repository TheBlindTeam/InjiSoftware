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

#include "gtkspell-win32.h"

#include <shlobj.h>

static HMODULE hmodule;

BOOL WINAPI
DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if(fdwReason == DLL_PROCESS_ATTACH)
    hmodule = hinstDLL;
  return TRUE;
}

/**
 * get_win32_prefix:
 *
 * Retreives the installation prefix.
 *
 * Return: The installation prefix.
 */
const gchar *
get_win32_prefix (void)
{
  static gchar retval[MAX_PATH];
  static gint beenhere = FALSE;
  gchar *temp_dir = 0;

  if (beenhere == FALSE)
    {
      temp_dir = g_win32_get_package_installation_directory_of_module ((gpointer) hmodule);
      if (temp_dir)
        g_strlcpy (retval, temp_dir, sizeof (retval));

      g_free (temp_dir);
      beenhere = TRUE;
    }

  return retval;
}

/**
 * get_win32_localedir:
 *
 * Retreives the path to the locale folder.
 *
 * Return: The path to the locale folder.
 */
const gchar *
get_win32_localedir (void)
{
  static gchar retval[MAX_PATH];
  static gint beenhere = FALSE;

  if (beenhere == FALSE)
    {
      g_strlcpy (retval, get_win32_prefix (), sizeof (retval));
      g_strlcat (retval, "\\share\\locale", sizeof (retval));
      beenhere = TRUE;
    }

  return retval;
}
