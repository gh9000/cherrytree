/*
 * ct_widgets.h
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

#include "ct_filesystem.h"

#include <gtkmm.h>
#include <libxml++/libxml++.h>
#include <sqlite3.h>

#include <unordered_map>
#include <memory>

class CtMDParser;
class CtClipboard;
class CtTmp
{
public:
    CtTmp() {}
    virtual ~CtTmp();
    fs::path getHiddenDirPath(const fs::path& visiblePath);
    fs::path getHiddenFilePath(const fs::path& visiblePath);

protected:
    std::unordered_map<std::string, gchar*> _mapHiddenDirs;
    std::unordered_map<std::string, gchar*> _mapHiddenFiles;
};

class CtMainWin;
class CtAnchoredWidgetState;
class CtStorageCache;

class CtAnchoredWidget : public Gtk::EventBox
{
public:
    CtAnchoredWidget(CtMainWin* pCtMainWin, const int charOffset, const std::string& justification);
    ~CtAnchoredWidget() override {}

    void insertInTextBuffer(Glib::RefPtr<Gsv::Buffer> rTextBuffer);
    Glib::RefPtr<Gtk::TextChildAnchor> getTextChildAnchor() { return _rTextChildAnchor; }

    virtual void apply_width_height(const int parentTextWidth) = 0;
    virtual void apply_syntax_highlighting(const bool forceReApply) = 0;
    virtual void to_xml(xmlpp::Element* p_node_parent, const int offset_adjustment, CtStorageCache* cache) = 0;
    virtual bool to_sqlite(sqlite3* pDb, const gint64 node_id, const int offset_adjustment, CtStorageCache* cache) = 0;
    virtual void set_modified_false() = 0;
    virtual CtAnchWidgType get_type() = 0;
    virtual std::shared_ptr<CtAnchoredWidgetState> get_state() = 0;

    void updateOffset(int charOffset) { _charOffset = charOffset; }
    void updateJustification(const std::string& justification) { _justification = justification; }
    void updateJustification(const Gtk::TextIter& textIter);

    int getOffset() const { return _charOffset; }
    const std::string& getJustification() const { return _justification; }

    bool operator<(const CtAnchoredWidget &other) { return getOffset() < other.getOffset(); }
    bool operator>(const CtAnchoredWidget &other) { return getOffset() > other.getOffset(); }

protected:
    void _on_size_allocate_frame(Gtk::Allocation& allocation);

protected:
    CtMainWin* _pCtMainWin;
    int _charOffset;
    std::string _justification;
    Gtk::Frame _frame;
    Gtk::Label _labelWidget;
    Glib::RefPtr<Gtk::TextChildAnchor> _rTextChildAnchor;
    Gtk::Allocation _lastAllocation;
};

class CtTreeView : public Gtk::TreeView
{
public:
    const inline static int TITLE_COL_NUM = 0;
    const inline static int AUX_ICON_COL_NUM = 1;

public:
    CtTreeView();
    virtual ~CtTreeView() {}

    void set_cursor_safe(const Gtk::TreeIter& iter);
    void set_tree_node_name_wrap_width(const bool wrap_enabled, const int wrap_width);
};

class CtApp;

class CtStatusIcon
{
public:
    CtStatusIcon(CtApp& ctApp);

    Gtk::StatusIcon* get();
    void ensure_menu_hidden();

private:
    CtApp& _ctApp;
    Glib::RefPtr<Gtk::StatusIcon> _rStatusIcon;
    std::unique_ptr<Gtk::Menu> _uStatusIconMenu;
};
