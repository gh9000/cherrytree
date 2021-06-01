/*
 * ct_text_view.h
 *
 * Copyright 2009-2021
 * Giuseppe Penone <giuspen@gmail.com>
 * Evgenii Gurianov <https://github.com/txe>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#pragma once

#include "ct_types.h"
#include "ct_column_edit.h"

#include <gtksourceviewmm.h>
#include <gspell/gspell.h>
#include <memory>
#include <unordered_map>

class CtMainWin;

class CtTextView : public Gsv::View
{
public:
    CtTextView(CtMainWin* pCtMainWin);

    CtMainWin*   getCtMainWin() { return _pCtMainWin; }

    void setup_for_syntax(const std::string& syntaxHighlighting); // pygtk: sourceview_set_properties
    void set_pixels_inside_wrap(int space_around_lines, int relative_wrapped_space);
    void set_selection_at_offset_n_delta(int offset,
                                         int delta,
                                         Glib::RefPtr<Gtk::TextBuffer> text_buffer = Glib::RefPtr<Gtk::TextBuffer>{});
    void list_change_level(Gtk::TextIter iter_insert, const CtListInfo& list_info, bool level_increase);
    void replace_text(const Glib::ustring& text, int start_offset, int end_offset);

    void for_event_after_double_click_button1(GdkEvent* event);
    void for_event_after_triple_click_button1(GdkEvent* event);
    void for_event_after_button_press(GdkEvent* event);
    void for_event_after_key_press(GdkEvent* event, const Glib::ustring& syntaxHighlighting);

    void cursor_and_tooltips_handler(int x, int y);
    void zoom_text(const bool is_increase, const std::string& syntaxHighlighting);
    void set_spell_check(bool allow_on);
    void synch_spell_check_change_from_gspell_right_click_menu();

    void set_buffer(const Glib::RefPtr<Gtk::TextBuffer>& buffer);
    void selection_update() {
        _columnEdit.selection_update();
    }
    void text_inserted(const Gtk::TextIter& pos, const Glib::ustring& text) {
        _columnEdit.text_inserted(pos, text);
    }
    void text_removed(const Gtk::TextIter& range_start, const Gtk::TextIter& range_end) {
        _columnEdit.text_removed(range_start, range_end);
    }
    bool own_insert_delete_active() {
        return _columnEdit.own_insert_delete_active();
    }
    guint64 get_todo_rotate_time() { return _todoRotateTime; }

private:
    bool          _apply_tag_try_link(Gtk::TextIter iter_end, int offset_cursor);
    Glib::ustring _get_former_line_indentation(Gtk::TextIter iter_start);
    void          _special_char_replace(gunichar special_char, Gtk::TextIter iter_start, Gtk::TextIter iter_insert);
    /// Replace the char between iter_start and iter_end with another one
    void          _special_char_replace(Glib::ustring special_char, Gtk::TextIter iter_start, Gtk::TextIter iter_end);

#ifdef MD_AUTO_REPLACEMENT
    bool          _markdown_filter_active();
#endif // MD_AUTO_REPLACEMENT

public:
    static const double TEXT_SCROLL_MARGIN;

private:
    static std::unordered_map<std::string, GspellChecker*> _static_spell_checkers;
    static GspellChecker* _get_spell_checker(const std::string& lang);

private:
#ifdef MD_AUTO_REPLACEMENT
    std::unique_ptr<CtMarkdownFilter> _md_handler;
#endif // MD_AUTO_REPLACEMENT
    CtMainWin*   _pCtMainWin;
    CtColumnEdit _columnEdit;
    guint32      _todoRotateTime{0};
};
