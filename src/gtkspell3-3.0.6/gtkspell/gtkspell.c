/* gtkspell - a spell-checking addon for GTK's TextView widget
 * Copyright (c) 2002 Evan Martin
 * Copyright (c) 2012-2013 Sandro Mani
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

#include "../config.h"
#include "gtkspell.h"
#include <string.h>
#include <libintl.h>
#include <locale.h>
#include <enchant.h>

#ifdef HAVE_ISO_CODES
#include "gtkspell-codetable.h"
#endif

#ifdef G_OS_WIN32
#include "gtkspell-win32.h"
#endif

#define _(String) dgettext (GETTEXT_PACKAGE, String)

#define GTK_SPELL_MISSPELLED_TAG "gtkspell-misspelled"
#define GTK_SPELL_OBJECT_KEY "gtkspell"

static const int debug = 0;
static const int quiet = 0;

static EnchantBroker *broker = NULL;
static int broker_ref_cnt = 0;
#ifdef HAVE_ISO_CODES
static int codetable_ref_cnt = 0;
#endif

static void gtk_spell_checker_dispose (GObject *object);
static void gtk_spell_checker_finalize (GObject *object);

enum
{
  LANGUAGE_CHANGED,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

enum
{
  PROP_0,
  PROP_DECODE_LANGUAGE_CODES
};

#define GTK_SPELL_CHECKER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_SPELL_TYPE_CHECKER, GtkSpellCheckerPrivate))

struct _GtkSpellCheckerPrivate
{
  GtkTextView *view;
  GtkTextBuffer *buffer;
  GtkTextTag *tag_highlight;
  GtkTextMark *mark_insert_start;
  GtkTextMark *mark_insert_end;
  GtkTextMark *mark_click;
  gboolean deferred_check;
  EnchantDict *speller;
  gchar *lang;
  gboolean decode_codes;
};

G_DEFINE_TYPE (GtkSpellChecker, gtk_spell_checker, G_TYPE_INITIALLY_UNOWNED)

static gboolean
gtk_spell_text_iter_forward_word_end (GtkTextIter *i)
{
  GtkTextIter iter;

  /* heuristic:
   * if we're on an singlequote/apostrophe and
   * if the next letter is alphanumeric,
   * this is an apostrophe (either single quote, or U+2019 = 8217. */

  if (!gtk_text_iter_forward_word_end (i))
    return FALSE;

  if (gtk_text_iter_get_char (i) != '\'' &&
      gtk_text_iter_get_char (i) != 8217)
    return TRUE;

  iter = *i;
  if (gtk_text_iter_forward_char (&iter) &&
      g_unichar_isalpha (gtk_text_iter_get_char (&iter)))
    return (gtk_text_iter_forward_word_end (i));

  return TRUE;
}

static gboolean
gtk_spell_text_iter_backward_word_start (GtkTextIter *i)
{
  GtkTextIter iter;

  if (!gtk_text_iter_backward_word_start (i))
    return FALSE;

  iter = *i;
  if (g_unichar_isalpha (gtk_text_iter_get_char (&iter)) &&
      gtk_text_iter_backward_char (&iter) &&
      (gtk_text_iter_get_char (&iter) == '\'' ||
       gtk_text_iter_get_char (&iter) == 8217))
    return (gtk_text_iter_backward_word_start (i));

  return TRUE;
}

#define gtk_text_iter_backward_word_start gtk_spell_text_iter_backward_word_start
#define gtk_text_iter_forward_word_end gtk_spell_text_iter_forward_word_end

static void
check_word (GtkSpellChecker *spell, GtkTextIter *start, GtkTextIter *end)
{
  char *text;
  text = gtk_text_buffer_get_text (spell->priv->buffer, start, end, FALSE);
  if (debug)
    g_print ("checking: %s\n", text);
  if (g_unichar_isdigit (*text) == FALSE && /* don't check numbers */
      enchant_dict_check (spell->priv->speller, text, strlen (text)) != 0)
    gtk_text_buffer_apply_tag (spell->priv->buffer, spell->priv->tag_highlight, start, end);
  g_free (text);
}

static void
print_iter (char *name, GtkTextIter *iter)
{
  g_print ("%1s[%d%c%c%c] ", name, gtk_text_iter_get_offset (iter),
           gtk_text_iter_starts_word (iter) ? 's' : ' ',
           gtk_text_iter_inside_word (iter) ? 'i' : ' ',
           gtk_text_iter_ends_word (iter) ? 'e' : ' ');
}

static void
check_range (GtkSpellChecker *spell, GtkTextIter start,
             GtkTextIter end, gboolean force_all)
{
  g_return_if_fail (spell->priv->speller != NULL); /* for check_word */

  /* we need to "split" on word boundaries.
   * luckily, pango knows what "words" are
   * so we don't have to figure it out. */

  GtkTextIter wstart, wend, cursor, precursor;
  gboolean inword, highlight;
  if (debug)
    {
      g_print ("check_range: ");
      print_iter ("s", &start);
      print_iter ("e", &end);
      g_print (" -> ");
    }

  if (gtk_text_iter_inside_word (&end))
    gtk_text_iter_forward_word_end (&end);
  if (!gtk_text_iter_starts_word (&start))
    {
      if (gtk_text_iter_inside_word (&start) ||
          gtk_text_iter_ends_word (&start))
        {
          gtk_text_iter_backward_word_start (&start);
        }
      else
        {
          /* if we're neither at the beginning nor inside a word,
           * me must be in some spaces.
           * skip forward to the beginning of the next word. */
          //gtk_text_buffer_remove_tag (buffer, tag_highlight, &start, &end);
          if (gtk_text_iter_forward_word_end (&start))
            gtk_text_iter_backward_word_start (&start);
        }
    }
  gtk_text_buffer_get_iter_at_mark (spell->priv->buffer, &cursor,
                                    gtk_text_buffer_get_insert (spell->priv->buffer));

  precursor = cursor;
  gtk_text_iter_backward_char (&precursor);
  highlight = gtk_text_iter_has_tag (&cursor, spell->priv->tag_highlight) ||
      gtk_text_iter_has_tag (&precursor, spell->priv->tag_highlight);

  gtk_text_buffer_remove_tag (spell->priv->buffer, spell->priv->tag_highlight, &start, &end);

  /* Fix a corner case when replacement occurs at beginning of buffer:
   * An iter at offset 0 seems to always be inside a word,
   * even if it's not.  Possibly a pango bug.
   */
  if (gtk_text_iter_get_offset (&start) == 0)
    {
      gtk_text_iter_forward_word_end (&start);
      gtk_text_iter_backward_word_start (&start);
    }

  if (debug)
    {
      print_iter ("s", &start);
      print_iter ("e", &end);
      g_print ("\n");
    }

  wstart = start;
  while (gtk_text_iter_compare (&wstart, &end) < 0)
    {
      /* move wend to the end of the current word. */
      wend = wstart;
      gtk_text_iter_forward_word_end (&wend);

      inword = (gtk_text_iter_compare (&wstart, &cursor) < 0) &&
               (gtk_text_iter_compare (&cursor, &wend) <= 0);

      if (inword && !force_all)
        {
          /* this word is being actively edited,
           * only check if it's already highligted,
           * otherwise defer this check until later. */
          if (highlight)
            check_word (spell, &wstart, &wend);
          else
            spell->priv->deferred_check = TRUE;
        }
      else
        {
          check_word (spell, &wstart, &wend);
          spell->priv->deferred_check = FALSE;
        }

      /* now move wend to the beginning of the next word, */
      gtk_text_iter_forward_word_end (&wend);
      gtk_text_iter_backward_word_start (&wend);
      /* make sure we've actually advanced
       * (we don't advance in some corner cases), */
      if (gtk_text_iter_equal (&wstart, &wend))
        break; /* we're done in these cases.. */
      /* and then pick this as the new next word beginning. */
      wstart = wend;
    }
}

static void
check_deferred_range (GtkSpellChecker *spell, gboolean force_all)
{
  GtkTextIter start, end;
  gtk_text_buffer_get_iter_at_mark (spell->priv->buffer, &start, spell->priv->mark_insert_start);
  gtk_text_buffer_get_iter_at_mark (spell->priv->buffer, &end, spell->priv->mark_insert_end);
  check_range (spell, start, end, force_all);
}

/* insertion works like this:
 *  - before the text is inserted, we mark the position in the buffer.
 *  - after the text is inserted, we see where our mark is and use that and
 *    the current position to check the entire range of inserted text.
 *
 * this may be overkill for the common case (inserting one character). */

static void
insert_text_before (GtkTextBuffer *buffer, GtkTextIter *iter,
                    gchar *text, gint len, GtkSpellChecker *spell)
{
  g_return_if_fail (buffer == spell->priv->buffer);

  gtk_text_buffer_move_mark (buffer, spell->priv->mark_insert_start, iter);
}

static void
insert_text_after (GtkTextBuffer *buffer, GtkTextIter *iter,
                   gchar *text, gint len, GtkSpellChecker *spell)
{
  g_return_if_fail (buffer == spell->priv->buffer);

  GtkTextIter start;

  if (debug)
    g_print ("insert\n");

  /* we need to check a range of text. */
  gtk_text_buffer_get_iter_at_mark (buffer, &start, spell->priv->mark_insert_start);
  check_range (spell, start, *iter, FALSE);

  gtk_text_buffer_move_mark (buffer, spell->priv->mark_insert_end, iter);
}

/* deleting is more simple:  we're given the range of deleted text.
 * after deletion, the start and end iters should be at the same position
 * (because all of the text between them was deleted!).
 * this means we only really check the words immediately bounding the
 * deletion.
 */

static void
delete_range_after (GtkTextBuffer *buffer, GtkTextIter *start,
                    GtkTextIter *end, GtkSpellChecker *spell)
{
  g_return_if_fail (buffer == spell->priv->buffer);

  if (debug)
    g_print ("delete\n");
  check_range (spell, *start, *end, FALSE);
}

static void
mark_set (GtkTextBuffer *buffer, GtkTextIter *iter,
          GtkTextMark *mark, GtkSpellChecker *spell)
{
  g_return_if_fail (buffer == spell->priv->buffer);

  /* if the cursor has moved and there is a deferred check so handle it now */
  if ((mark == gtk_text_buffer_get_insert (buffer)) && spell->priv->deferred_check)
    check_deferred_range (spell, FALSE);
}

static void
get_word_extents_from_mark (GtkTextBuffer *buffer, GtkTextIter *start,
                            GtkTextIter *end, GtkTextMark *mark)
{
  gtk_text_buffer_get_iter_at_mark (buffer, start, mark);
  if (!gtk_text_iter_starts_word (start))
    gtk_text_iter_backward_word_start (start);
  *end = *start;
  if (gtk_text_iter_inside_word (end))
    gtk_text_iter_forward_word_end (end);
}

static void
add_to_dictionary (GtkWidget *menuitem, GtkSpellChecker *spell)
{
  char *word;
  GtkTextIter start, end;

  get_word_extents_from_mark (spell->priv->buffer, &start, &end, spell->priv->mark_click);
  word = gtk_text_buffer_get_text (spell->priv->buffer, &start, &end, FALSE);

  enchant_dict_add_to_pwl (spell->priv->speller, word, strlen (word));

  gtk_spell_checker_recheck_all (spell);

  g_free (word);
}

static void
ignore_all (GtkWidget *menuitem, GtkSpellChecker *spell)
{
  char *word;
  GtkTextIter start, end;

  get_word_extents_from_mark (spell->priv->buffer, &start, &end, spell->priv->mark_click);
  word = gtk_text_buffer_get_text (spell->priv->buffer, &start, &end, FALSE);

  enchant_dict_add_to_session (spell->priv->speller, word, strlen (word));

  gtk_spell_checker_recheck_all (spell);

  g_free (word);
}

static void
replace_word (GtkWidget *menuitem, GtkSpellChecker *spell)
{
  char *oldword;
  const char *newword;
  GtkTextIter start, end;

  get_word_extents_from_mark (spell->priv->buffer, &start, &end, spell->priv->mark_click);
  oldword = gtk_text_buffer_get_text (spell->priv->buffer, &start, &end, FALSE);
  newword = gtk_label_get_text (GTK_LABEL (gtk_bin_get_child (GTK_BIN (menuitem))));

  if (debug)
    {
      g_print ("old word: '%s'\n", oldword);
      print_iter ("s", &start);
      print_iter ("e", &end);
      g_print ("\nnew word: '%s'\n", newword);
    }

  gtk_text_buffer_begin_user_action (spell->priv->buffer);
  gtk_text_buffer_delete (spell->priv->buffer, &start, &end);
  gtk_text_buffer_insert (spell->priv->buffer, &start, newword, -1);
  gtk_text_buffer_end_user_action (spell->priv->buffer);

  enchant_dict_store_replacement (spell->priv->speller,
                                  oldword, strlen (oldword),
                                  newword, strlen (newword));

  g_free (oldword);
}

/* This function populates suggestions at the top of the passed menu */
static void
add_suggestion_menus (GtkSpellChecker *spell, const char *word, GtkWidget *topmenu)
{
  g_return_if_fail (spell->priv->speller != NULL);

  GtkWidget *menu;
  GtkWidget *mi;
  char **suggestions;
  size_t n_suggs, i;
  char *label;

  menu = topmenu;

  gint menu_position = 0;

  suggestions = enchant_dict_suggest (spell->priv->speller, word,
                                      strlen (word), &n_suggs);

  if (suggestions == NULL || !n_suggs)
    {
      /* no suggestions.  put something in the menu anyway... */
      GtkWidget *label;
      label = gtk_label_new ("");
      gtk_label_set_markup (GTK_LABEL (label), _("<i>(no suggestions)</i>"));

      mi = gtk_menu_item_new ();
      gtk_container_add (GTK_CONTAINER (mi), label);
      gtk_widget_show_all (mi);
      gtk_menu_shell_insert (GTK_MENU_SHELL (menu), mi, menu_position++);
    }
  else
    {
      /* build a set of menus with suggestions. */
      gboolean inside_more_submenu = FALSE;
      for (i = 0; i < n_suggs; i++ )
        {
          if (i > 0 && i % 10 == 0)
            {
              inside_more_submenu = TRUE;
              mi = gtk_menu_item_new_with_label (_("More..."));
              gtk_widget_show (mi);
              gtk_menu_shell_insert (GTK_MENU_SHELL (menu), mi, menu_position++);

              menu = gtk_menu_new ();
              gtk_menu_item_set_submenu (GTK_MENU_ITEM (mi), menu);
            }
          mi = gtk_menu_item_new_with_label (suggestions[i]);
          g_signal_connect (mi, "activate", G_CALLBACK (replace_word), spell);
          gtk_widget_show (mi);
          if (inside_more_submenu)
            gtk_menu_shell_append (GTK_MENU_SHELL (menu), mi);
          else
            gtk_menu_shell_insert (GTK_MENU_SHELL (menu), mi, menu_position++);
        }
    }

  if (suggestions)
    enchant_dict_free_string_list (spell->priv->speller, suggestions);

  /* + Add to Dictionary */
  label = g_strdup_printf (_("Add \"%s\" to Dictionary"), word);
#if GTK_CHECK_VERSION(3,9,0)
  mi = gtk_menu_item_new_with_label (label);
#else
  mi = gtk_image_menu_item_new_with_label (label);
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mi),
                 gtk_image_new_from_stock (GTK_STOCK_ADD, GTK_ICON_SIZE_MENU));
#endif
  g_free (label);
  g_signal_connect (mi, "activate", G_CALLBACK (add_to_dictionary), spell);
  gtk_widget_show_all (mi);
  gtk_menu_shell_insert (GTK_MENU_SHELL (topmenu), mi, menu_position++);

  /* - Ignore All */
#if GTK_CHECK_VERSION(3,9,0)
  mi = gtk_menu_item_new_with_label (_("Ignore All"));
#else
  mi = gtk_image_menu_item_new_with_label (_("Ignore All"));
  gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (mi),
              gtk_image_new_from_stock (GTK_STOCK_REMOVE, GTK_ICON_SIZE_MENU));
#endif
  g_signal_connect (mi, "activate", G_CALLBACK (ignore_all), spell);
  gtk_widget_show_all (mi);
  gtk_menu_shell_insert (GTK_MENU_SHELL (topmenu), mi, menu_position++);
}

static GtkWidget*
build_suggestion_menu (GtkSpellChecker *spell, const char *word)
{
  GtkWidget *topmenu;
  topmenu = gtk_menu_new ();
  add_suggestion_menus (spell, word, topmenu);

  return topmenu;
}

static void
language_change_callback (GtkCheckMenuItem *mi, GtkSpellChecker* spell)
{
  if (gtk_check_menu_item_get_active (mi))
    {
      GError* error = NULL;
      gchar *name;
      g_object_get (G_OBJECT (mi), "name", &name, NULL);
      gtk_spell_checker_set_language (spell, name, &error);
      g_signal_emit (spell, signals[LANGUAGE_CHANGED], 0, spell->priv->lang);
      g_free (name);
    }
}

struct _languages_cb_struct { GList *langs; };

static void
dict_describe_cb (const char * const lang_tag,
                  const char * const provider_name,
                  const char * const provider_desc,
                  const char * const provider_file,
                  void * user_data)
{
  struct _languages_cb_struct *languages_cb_struct = (struct _languages_cb_struct *)user_data;

  languages_cb_struct->langs = g_list_insert_sorted (
      languages_cb_struct->langs, g_strdup (lang_tag),
      (GCompareFunc) strcmp);
}

static GtkWidget*
build_languages_menu (GtkSpellChecker *spell)
{
  GtkWidget *active_item = NULL, *menu = gtk_menu_new ();
  GList *langs;
  GtkWidget *mi;
  GSList *menu_group = NULL;

  struct _languages_cb_struct languages_cb_struct;
  languages_cb_struct.langs = NULL;

  enchant_broker_list_dicts (broker, dict_describe_cb, &languages_cb_struct);

  langs = languages_cb_struct.langs;

  for (; langs; langs = langs->next)
    {
      gchar *lang_tag = langs->data;
#ifdef HAVE_ISO_CODES
      if (spell->priv->decode_codes == TRUE)
        {
          const gchar *lang_name = "\0";
          const gchar *country_name = "\0";
          gchar *label;
          codetable_lookup (lang_tag, &lang_name, &country_name);
          if (strlen (country_name) != 0)
            label = g_strdup_printf ("%s (%s)", lang_name, country_name);
          else
            label = g_strdup_printf ("%s", lang_name);
          mi = gtk_radio_menu_item_new_with_label (menu_group, label);
          g_free (label);
        }
      else
#endif
        mi = gtk_radio_menu_item_new_with_label (menu_group, lang_tag);
      menu_group = gtk_radio_menu_item_get_group (GTK_RADIO_MENU_ITEM (mi));

      g_object_set (G_OBJECT (mi), "name", lang_tag, NULL);
      if (strcmp (spell->priv->lang, lang_tag) == 0)
        active_item = mi;
      else
        g_signal_connect (mi, "activate",
                          G_CALLBACK (language_change_callback), spell);
      gtk_widget_show (mi);
      gtk_menu_shell_append (GTK_MENU_SHELL (menu), mi);

      g_free (lang_tag);
    }
  if (active_item)
    gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (active_item), TRUE);

  g_list_free (languages_cb_struct.langs);

  return menu;
}

static void
populate_popup (GtkTextView *textview, GtkMenu *menu, GtkSpellChecker *spell)
{
  g_return_if_fail (spell->priv->view == textview);

  GtkWidget *mi;
  GtkTextIter start, end;
  char *word;

  /* menu separator comes first. */
  mi = gtk_separator_menu_item_new ();
  gtk_widget_show (mi);
  gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), mi);

  /* on top: language selection */
  mi = gtk_menu_item_new_with_label (_("Languages"));
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (mi), build_languages_menu (spell));
  gtk_widget_show_all (mi);
  gtk_menu_shell_prepend (GTK_MENU_SHELL (menu), mi);

  /* we need to figure out if they picked a misspelled word. */
  get_word_extents_from_mark (spell->priv->buffer, &start, &end, spell->priv->mark_click);

  /* if our highlight algorithm ever messes up,
   * this isn't correct, either. */
  if (!gtk_text_iter_has_tag (&start, spell->priv->tag_highlight))
    return; /* word wasn't misspelled. */

  /* then, on top of it, the suggestions */
  word = gtk_text_buffer_get_text (spell->priv->buffer, &start, &end, FALSE);
  add_suggestion_menus (spell, word, GTK_WIDGET (menu));
  g_free (word);
}

/* when the user right-clicks on a word, they want to check that word.
 * here, we do NOT  move the cursor to the location of the clicked-upon word
 * since that prevents the use of edit functions on the context menu. */
static gboolean
button_press_event (GtkTextView *view, GdkEventButton *event, GtkSpellChecker *spell)
{
  g_return_val_if_fail (spell->priv->view == view, FALSE);
  g_return_val_if_fail (spell->priv->buffer == gtk_text_view_get_buffer (view), FALSE);

  if (event->button == 3)
    {
      gint x, y;
      GtkTextIter iter;

      /* handle deferred check if it exists */
      if (spell->priv->deferred_check)
        check_deferred_range (spell, TRUE);

      gtk_text_view_window_to_buffer_coords (view, GTK_TEXT_WINDOW_TEXT,
                                             event->x, event->y, &x, &y);
      gtk_text_view_get_iter_at_location (view, &iter, x, y);
      gtk_text_buffer_move_mark (spell->priv->buffer, spell->priv->mark_click, &iter);
    }
  return FALSE; /* false: let gtk process this event, too.
                 * we don't want to eat any events. */
}

/* This event occurs when the popup menu is requested through a key-binding
 * (Menu Key or <shift>+F10 by default).  In this case we want to set
 * spell->priv->mark_click to the cursor position. */
static gboolean
popup_menu_event (GtkTextView *view, GtkSpellChecker *spell)
{
  g_return_val_if_fail (spell->priv->view == view, FALSE);

  GtkTextIter iter;

  gtk_text_buffer_get_iter_at_mark (spell->priv->buffer, &iter,
                                   gtk_text_buffer_get_insert (spell->priv->buffer));
  gtk_text_buffer_move_mark (spell->priv->buffer, spell->priv->mark_click, &iter);
  return FALSE; /* false: let gtk process this event, too. */
}

static void
set_lang_from_dict (const char * const lang_tag,
                    const char * const provider_name,
                    const char * const provider_desc,
                    const char * const provider_dll_file,
                    void * user_data)
{
  GtkSpellChecker *spell = user_data;

  g_free (spell->priv->lang);
  spell->priv->lang = g_strdup (lang_tag);
}

static gboolean
set_language_internal (GtkSpellChecker *spell, const gchar *lang, GError **error)
{
  EnchantDict *dict;

  if (lang == NULL)
    {
      lang = g_getenv ("LANG");
      if (lang)
        {
          if ((strcmp (lang, "C") == 0) || (strcmp (lang, "c") == 0))
            lang = NULL;
          else if (lang[0] == 0)
            lang = NULL;
        }
    }

  if (!lang)
    lang = "en";

  dict = enchant_broker_request_dict (broker, lang);

  if (!dict)
    {
      g_set_error (error, GTK_SPELL_ERROR, GTK_SPELL_ERROR_BACKEND,
                   _("enchant error for language: %s"), lang);
      return FALSE;
    }

  if (spell->priv->speller)
    enchant_broker_free_dict (broker, spell->priv->speller);
  spell->priv->speller = dict;

  enchant_dict_describe (dict, set_lang_from_dict, spell);

  return TRUE;
}

/* changes the buffer
 * a NULL buffer is acceptable and will only release the current one */
static void
set_buffer (GtkSpellChecker *spell, GtkTextBuffer *buffer)
{
  GtkTextIter start, end;

  if (spell->priv->buffer)
    {
      g_signal_handlers_disconnect_matched (spell->priv->buffer, G_SIGNAL_MATCH_DATA,
                                            0, 0, NULL, NULL, spell);

      gtk_text_buffer_get_bounds (spell->priv->buffer, &start, &end);
      gtk_text_buffer_remove_tag (spell->priv->buffer, spell->priv->tag_highlight,
                                  &start, &end);
      spell->priv->tag_highlight = NULL;

      gtk_text_buffer_delete_mark (spell->priv->buffer, spell->priv->mark_insert_start);
      spell->priv->mark_insert_start = NULL;
      gtk_text_buffer_delete_mark (spell->priv->buffer, spell->priv->mark_insert_end);
      spell->priv->mark_insert_end = NULL;
      gtk_text_buffer_delete_mark (spell->priv->buffer, spell->priv->mark_click);
      spell->priv->mark_click = NULL;

      g_object_unref (spell->priv->buffer);
    }

  spell->priv->buffer = buffer;

  if (spell->priv->buffer)
    {
      g_object_ref (spell->priv->buffer);

      g_signal_connect (spell->priv->buffer, "insert-text",
                        G_CALLBACK (insert_text_before), spell);
      g_signal_connect_after (spell->priv->buffer, "insert-text",
                        G_CALLBACK (insert_text_after), spell);
      g_signal_connect_after (spell->priv->buffer, "delete-range",
                        G_CALLBACK (delete_range_after), spell);
      g_signal_connect (spell->priv->buffer, "mark-set",
                        G_CALLBACK (mark_set), spell);

      GtkTextTagTable *tagtable = gtk_text_buffer_get_tag_table (spell->priv->buffer);
      spell->priv->tag_highlight = gtk_text_tag_table_lookup (tagtable,
                                                     GTK_SPELL_MISSPELLED_TAG);

      if (spell->priv->tag_highlight == NULL)
        {
          spell->priv->tag_highlight = gtk_text_buffer_create_tag (spell->priv->buffer,
                                         GTK_SPELL_MISSPELLED_TAG, "underline",
                                         PANGO_UNDERLINE_ERROR, NULL);
        }

      /* we create the mark here, but we don't use it until text is
       * inserted, so we don't really care where iter points.  */
      gtk_text_buffer_get_bounds (spell->priv->buffer, &start, &end);
      spell->priv->mark_insert_start = gtk_text_buffer_create_mark (spell->priv->buffer,
                                        "gtkspell-insert-start", &start, TRUE);
      spell->priv->mark_insert_end = gtk_text_buffer_create_mark (spell->priv->buffer,
                                          "gtkspell-insert-end", &start, TRUE);
      spell->priv->mark_click = gtk_text_buffer_create_mark (spell->priv->buffer,
                                               "gtkspell-click", &start, TRUE);

      spell->priv->deferred_check = FALSE;

      /* now check the entire text buffer. */
      gtk_spell_checker_recheck_all (spell);
    }
}

static void
buffer_changed (GtkTextView *view, GParamSpec *pspec, GtkSpellChecker *spell)
{
  g_return_if_fail (spell->priv->view == view);

  GtkTextBuffer *buf = gtk_text_view_get_buffer (view);
  if (!buf)
    gtk_spell_checker_detach (spell);
  else
    set_buffer (spell, buf);
}

static void
gtk_spell_checker_set_property (GObject *object,
                                guint propid,
                                const GValue *value,
                                GParamSpec *pspec)
{
  GtkSpellChecker *spell = GTK_SPELL_CHECKER (object);

  switch (propid)
    {
    case PROP_DECODE_LANGUAGE_CODES:
      spell->priv->decode_codes = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, propid, pspec);
      break;
    }
}

static void
gtk_spell_checker_get_property (GObject *object,
                                guint propid,
                                GValue *value,
                                GParamSpec *pspec)
{
  GtkSpellChecker *spell = GTK_SPELL_CHECKER (object);

  switch (propid)
    {
    case PROP_DECODE_LANGUAGE_CODES:
      g_value_set_boolean (value, spell->priv->decode_codes);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, propid, pspec);
      break;
    }
}

static void
gtk_spell_checker_class_init (GtkSpellCheckerClass *klass)
{
  g_type_class_add_private (klass, sizeof (GtkSpellCheckerPrivate));
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->dispose = gtk_spell_checker_dispose;
  object_class->finalize = gtk_spell_checker_finalize;
  object_class->set_property = gtk_spell_checker_set_property;
  object_class->get_property = gtk_spell_checker_get_property;

  /**
   * GtkSpellChecker::language-changed:
   * @spell: the #GtkSpellChecker object which received the signal.
   * @lang: the new language which was selected.
   *
   * The ::language-changed signal is emitted when the user selects
   * a new spelling language from the context menu.
   *
   */
  signals[LANGUAGE_CHANGED] = g_signal_new ("language-changed",
                      G_OBJECT_CLASS_TYPE (object_class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (GtkSpellCheckerClass, language_changed),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE,
                      1,
                      G_TYPE_STRING | G_SIGNAL_TYPE_STATIC_SCOPE);

  /**
   * GtkSpellChecker::decode-language-codes:
   *
   * Whether to show decoded language codes in the context menu
   * (requires the iso-codes package).
   */
  g_object_class_install_property (object_class, PROP_DECODE_LANGUAGE_CODES,
        g_param_spec_boolean ("decode-language-codes",
                              "Decode language codes",
                              "Whether to show decoded language codes in the "\
                              "context menu (requires the iso-codes package).",
                              FALSE,
                              G_PARAM_READWRITE));
}

static void
gtk_spell_checker_init (GtkSpellChecker *self)
{
  self->priv = GTK_SPELL_CHECKER_GET_PRIVATE (self);
  self->priv->view = NULL;
  self->priv->buffer = NULL;
  self->priv->tag_highlight = NULL;
  self->priv->mark_insert_start = NULL;
  self->priv->mark_insert_end = NULL;
  self->priv->mark_click = NULL;
  self->priv->deferred_check = FALSE;
  self->priv->speller = NULL;
  self->priv->lang = NULL;

#ifdef ENABLE_NLS
  bindtextdomain (PACKAGE_NAME, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (PACKAGE_NAME, "UTF-8");
#endif

  if (!broker)
    {
      broker = enchant_broker_init ();
      broker_ref_cnt = 0;
    }
  broker_ref_cnt++;

#ifdef HAVE_ISO_CODES
  if (codetable_ref_cnt == 0)
    codetable_init ();
  codetable_ref_cnt++;
#endif

  set_language_internal (self, NULL, NULL);
}

static void
gtk_spell_checker_dispose (GObject *object)
{
  GtkSpellChecker *spell = GTK_SPELL_CHECKER (object);

  gtk_spell_checker_detach (spell);

  G_INITIALLY_UNOWNED_CLASS (gtk_spell_checker_parent_class)->dispose (object);
}

static void
gtk_spell_checker_finalize (GObject *object)
{
  GtkSpellChecker *spell = GTK_SPELL_CHECKER (object);

  if (broker)
    {
      if (spell->priv->speller)
        enchant_broker_free_dict (broker, spell->priv->speller);
      broker_ref_cnt--;
      if (broker_ref_cnt == 0)
        {
          enchant_broker_free (broker);
          broker = NULL;
        }

#ifdef HAVE_ISO_CODES
      codetable_ref_cnt--;
      if (codetable_ref_cnt == 0)
        codetable_free ();
#endif

    }

  g_free (spell->priv->lang);

  G_INITIALLY_UNOWNED_CLASS (gtk_spell_checker_parent_class)->finalize (object);
}

/**
 * gtk_spell_checker_new:
 *
 * Create a new #GtkSpellChecker object.
 *
 * Returns: a new #GtkSpellChecker object.
 */
GtkSpellChecker*
gtk_spell_checker_new (void)
{
  return g_object_new (GTK_SPELL_TYPE_CHECKER, NULL);
}

/**
 * gtk_spell_checker_attach:
 * @spell: A #GtkSpellChecker.
 * @view: The #GtkTextView to attach to.
 *
 * Attach #GtkSpellChecker object to @view.
 *
 * Note: Please read the tutorial section of the documentation to make sure
 * you don't leak references!
 *
 * Returns: TRUE on success, FALSE on failure.
 */
gboolean
gtk_spell_checker_attach (GtkSpellChecker *spell, GtkTextView *view)
{
  g_return_val_if_fail (GTK_SPELL_IS_CHECKER (spell), FALSE);
  g_return_val_if_fail (GTK_IS_TEXT_VIEW (view), FALSE);
  g_return_val_if_fail (gtk_text_view_get_buffer (view), FALSE);
  g_return_val_if_fail (spell->priv->view == NULL, FALSE);

  /* ensure no existing instance is attached */
  GtkSpellChecker *attached;
  attached = g_object_get_data (G_OBJECT (view), GTK_SPELL_OBJECT_KEY);
  g_return_val_if_fail (attached == NULL, FALSE);

  /* attach to the widget */
  spell->priv->view = view;
  g_object_ref (view);
  g_object_ref_sink (spell);

  g_object_set_data (G_OBJECT (view), GTK_SPELL_OBJECT_KEY, spell);

  g_signal_connect_swapped (view, "destroy",
                            G_CALLBACK (gtk_spell_checker_detach), spell);
  g_signal_connect (view, "button-press-event",
                    G_CALLBACK (button_press_event), spell);
  g_signal_connect (view, "populate-popup",
                    G_CALLBACK (populate_popup), spell);
  g_signal_connect (view, "popup-menu",
                    G_CALLBACK (popup_menu_event), spell);
  g_signal_connect (view, "notify::buffer",
                    G_CALLBACK (buffer_changed), spell);

  set_buffer (spell, gtk_text_view_get_buffer (view));

  return TRUE;
}

/**
 * gtk_spell_checker_detach:
 * @spell: A #GtkSpellChecker.
 *
 * Detaches this #GtkSpellChecker from its #GtkTextView.  Use
 * gtk_spell_checker_get_from_text_view () to retrieve a #GtkSpellChecker from
 * a #GtkTextView. If the #GtkSpellChecker is not attached to any #GtkTextView,
 * the function silently exits.
 *
 * Note: if the #GtkSpellChecker is owned by the #GtkTextView, you must
 * take a reference to it to prevent it from being automatically destroyed.
 * Please read the tutorial section of the documentation!
 */
void
gtk_spell_checker_detach (GtkSpellChecker *spell)
{
  g_return_if_fail (GTK_SPELL_IS_CHECKER (spell));
  if (spell->priv->view == NULL)
    return;

  g_signal_handlers_disconnect_matched (spell->priv->view, G_SIGNAL_MATCH_DATA,
        0, 0, NULL, NULL, spell);

  g_object_set_data (G_OBJECT (spell->priv->view), GTK_SPELL_OBJECT_KEY, NULL);

  g_object_unref (spell->priv->view);
  spell->priv->view = NULL;
  set_buffer (spell, NULL);
  spell->priv->deferred_check = FALSE;
  g_object_unref (spell);
}

/**
 * gtk_spell_checker_get_from_text_view:
 * @view: A #GtkTextView.
 *
 * Retrieves the #GtkSpellChecker object attached to a text view.
 *
 * Returns: (transfer none): the #GtkSpellChecker object, or %NULL if there is no #GtkSpellChecker
 * attached to @view.
 */
GtkSpellChecker*
gtk_spell_checker_get_from_text_view (GtkTextView *view)
{
  g_return_val_if_fail (GTK_IS_TEXT_VIEW (view), NULL);
  return g_object_get_data (G_OBJECT (view), GTK_SPELL_OBJECT_KEY);
}

/**
 * gtk_spell_checker_get_language:
 * @spell: a #GtkSpellChecker
 *
 * Fetches the current language.
 *
 * Returns: the current language. This string is
 * owned by the spell object and must not be modified or freed.
 **/
const gchar*
gtk_spell_checker_get_language (GtkSpellChecker *spell)
{
  g_return_val_if_fail (GTK_SPELL_IS_CHECKER (spell), NULL);

  return spell->priv->lang;
}

/**
 * gtk_spell_checker_get_language_list:
 *
 * Requests the list of available languages from the enchant broker.
 *
 * Returns: (transfer full) (element-type utf8): a #GList of the available languages.
 * Use g_list_free to free the list after use.
 *
 * Since: 3.0.3
 */
GList*
gtk_spell_checker_get_language_list (void)
{
  struct _languages_cb_struct languages_cb_struct;

  if (!broker)
    {
      broker = enchant_broker_init();
      broker_ref_cnt = 0;
    }

  languages_cb_struct.langs = NULL;
  enchant_broker_list_dicts(broker, dict_describe_cb, &languages_cb_struct);

  if (broker_ref_cnt == 0)
    {
      enchant_broker_free (broker);
      broker = NULL;
    }

  return languages_cb_struct.langs;
}

/**
 * gtk_spell_checker_decode_language_code:
 * @lang: The language locale specifier (i.e. "en_US").
 *
 * Translates the language code to a human readable format
 * (i.e. "en_US" -> "English (United States)").
 * Note: If the iso-codes package is not available, the unchanged code is
 * returned.
 *
 * Returns: (transfer full): The translated language specifier. Use g_free to
 * free the returned string after use.
 *
 * Since: 3.0.3
 */
gchar*
gtk_spell_checker_decode_language_code (const gchar *lang)
{
#ifdef HAVE_ISO_CODES
  const gchar *lang_name = "\0";
  const gchar *country_name = "\0";
  if (codetable_ref_cnt == 0)
    codetable_init ();
  codetable_lookup (lang, &lang_name, &country_name);
  if (codetable_ref_cnt == 0)
    codetable_free ();
  if (strlen (country_name) != 0)
    return g_strdup_printf ("%s (%s)", lang_name, country_name);
  else
    return g_strdup_printf ("%s", lang_name);
#else
  return g_strdup (lang);
#endif
}

/**
 * gtk_spell_checker_set_language:
 * @spell: The #GtkSpellChecker object.
 * @lang: (allow-none): The language to use, as a locale specifier (i.e. "en_US").
 * If #NULL, attempt to use the default system locale (LANG).
 * @error: (out) (allow-none): Return location for error.
 *
 * Set the language on @spell to @lang, possibily returning an error in
 * @error.
 *
 * Returns: FALSE if there was an error.
 */
gboolean
gtk_spell_checker_set_language (GtkSpellChecker *spell, const gchar *lang, GError **error)
{
  g_return_val_if_fail (GTK_SPELL_IS_CHECKER (spell), FALSE);

  if (error)
    g_return_val_if_fail (*error == NULL, FALSE);

  gboolean ret = set_language_internal (spell, lang, error);
  if (ret)
    gtk_spell_checker_recheck_all (spell);

  return ret;
}

/**
 * gtk_spell_checker_recheck_all:
 * @spell: The #GtkSpellChecker object.
 *
 * Recheck the spelling in the entire buffer.
 */
void
gtk_spell_checker_recheck_all (GtkSpellChecker *spell)
{
  g_return_if_fail (GTK_SPELL_IS_CHECKER (spell));

  GtkTextIter start, end;

  if (spell->priv->buffer)
    {
      gtk_text_buffer_get_bounds (spell->priv->buffer, &start, &end);
      check_range (spell, start, end, TRUE);
    }
}

/**
 * gtk_spell_checker_get_suggestions_menu:
 * @spell: A #GtkSpellChecker.
 * @iter: Textiter of position in buffer to be corrected if necessary.
 *
 * Retrieves a submenu of replacement spellings, or NULL if the word at @iter is
 * not misspelt.
 *
 * Returns: (transfer full): the #GtkMenu widget, or %NULL if there is no need for a menu
 */
GtkWidget*
gtk_spell_checker_get_suggestions_menu (GtkSpellChecker *spell, GtkTextIter *iter)
{
  g_return_val_if_fail (GTK_SPELL_IS_CHECKER (spell), NULL);
  g_return_val_if_fail (iter != NULL, NULL);

  GtkWidget *submenu = NULL;
  GtkTextIter start, end;

  start = *iter;
  /* use the same lazy test, with same risk, as does the default menu arrangement */
  if (gtk_text_iter_has_tag (&start, spell->priv->tag_highlight))
    {
      /* word was mis-spelt */
      gchar *badword;
      /* in case a fix is requested, move the attention-point */
      gtk_text_buffer_move_mark (spell->priv->buffer, spell->priv->mark_click, iter);
      if (!gtk_text_iter_starts_word (&start))
        gtk_text_iter_backward_word_start (&start);
      end = start;
      if (gtk_text_iter_inside_word (&end))
        gtk_text_iter_forward_word_end (&end);
      badword = gtk_text_buffer_get_text (spell->priv->buffer, &start, &end, FALSE);

      submenu = build_suggestion_menu (spell, badword);
      gtk_widget_show (submenu);

      g_free (badword);
    }
  return submenu;
}

GQuark
gtk_spell_error_quark (void)
{
  static GQuark q = 0;
  if (q == 0)
    q = g_quark_from_static_string ("gtkspell-error-quark");
  return q;
}

GType
gtk_spell_error_get_type (void)
{
  static GType etype = 0;

  if (G_UNLIKELY(etype == 0)) {
    static const GEnumValue values[] = {
      { GTK_SPELL_ERROR_BACKEND, "GTK_SPELL_ERROR_BACKEND", "backend" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static (g_intern_static_string ("GtkSpellError"), values);
  }
  return etype;
}

