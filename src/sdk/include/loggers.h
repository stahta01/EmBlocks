/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */
/*
    This file is part of Em::Blocks.

    Copyright (c) 2011-2013 Em::Blocks

    Em::Blocks is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Em::Blocks is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Em::Blocks.  If not, see <http://www.gnu.org/licenses/>.

	@version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:
*/

#ifndef LOGGERS_H
#define LOGGERS_H

#include "logger.h"

#include <wx/colour.h>
#include <wx/font.h>
#include <wx/ffile.h>
#include <wx/textctrl.h>

class wxListCtrl;

// Helper function which blends a colour with the default window text colour,
// so that text will be readable in bright and dark colour schemes
wxColour BlendTextColour(wxColour col);

class DLLIMPORT StdoutLogger : public Logger
{
public:
    virtual void Append(const wxString& msg, Logger::level lv)
    {
        fputs(wxSafeConvertWX2MB(msg), lv < error ? stdout : stderr);
        fputs(::newline_string.mb_str(), lv < error ? stdout : stderr);
    }
};

class DLLIMPORT FileLogger : public Logger
{
protected:
    wxFFile f;
public:
    FileLogger(const wxString& filename) : f(filename, _T("wb")) {};
    FileLogger() {};

    virtual void Append(const wxString& msg, Logger::level /*lv*/)
    {
        fputs(wxSafeConvertWX2MB(msg), f.fp());
        fputs(::newline_string.mb_str(), f.fp());
    };

    virtual void Open(const wxString& filename) { Close(); f.Open(filename, _T("wb")); };
    virtual void Close(){ if(f.IsOpened()) f.Close(); };

};

struct CSS
{
    wxString caption;
    wxString info;
    wxString warning;
    wxString success;
    wxString error;
    wxString critical;
    wxString failure;
    wxString pagetitle;
    wxString spacer;
    wxString asterisk;

    CSS();
    operator wxString();
};

class DLLIMPORT HTMLFileLogger : public FileLogger
{
    CSS css;
public:
    HTMLFileLogger(const wxString& filename);
    void SetCSS(const CSS& in_css) { css = in_css; };

    virtual void Append(const wxString& msg, Logger::level lv);
    virtual void Open(const wxString& filename);
    virtual void Close();
};

class DLLIMPORT TextCtrlLogger : public Logger
{
protected:

    wxTextCtrl* control;
    bool fixed;
    wxTextAttr style[num_levels];

public:
    TextCtrlLogger(bool fixedPitchFont = false);
    ~TextCtrlLogger();

    virtual void CopyContentsToClipboard(bool selectionOnly = false);
    virtual void UpdateSettings();
    virtual void Append(const wxString& msg, Logger::level lv = info);
    virtual void Clear();
    virtual wxWindow* CreateControl(wxWindow* parent);
};


class DLLIMPORT TimestampTextCtrlLogger : public TextCtrlLogger
{
public:
    virtual void Append(const wxString& msg, Logger::level lv = info);
};

class DLLIMPORT ListCtrlLogger : public Logger
{
protected:

    wxListCtrl* control;
    bool fixed;
    wxArrayString titles;
    wxArrayInt widths;

    struct ListStyles
    {
        wxFont font;
        wxColour colour;
    };
    ListStyles style[num_levels];

    wxString GetItemAsText(long item) const;
public:

    ListCtrlLogger(const wxArrayString& titles, const wxArrayInt& widths, bool fixedPitchFont = false);
    ~ListCtrlLogger();

    virtual void CopyContentsToClipboard(bool selectionOnly = false);
    virtual void UpdateSettings();
    virtual void Append(const wxString& msg, Logger::level lv = info);
    virtual void Append(const wxArrayString& colValues, Logger::level lv = info);
    virtual size_t GetItemsCount() const;
    virtual void Clear();
    virtual wxWindow* CreateControl(wxWindow* parent);
};


#endif // LOGGERS_H
