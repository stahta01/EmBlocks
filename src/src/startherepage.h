/*
 * This file is part of the code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
*/
/*
    Copyright (C) Em::Blocks 2011-2013

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


    @version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:

 */
#ifndef STARTHEREPAGE_H
#define STARTHEREPAGE_H

#ifndef CB_PRECOMP
    #include <editorbase.h>
#endif

extern const wxString g_StartHereTitle;

class wxHtmlWindow;
class wxHtmlLinkInfo;

class StartHerePage : public EditorBase
{
    wxString revInfo;

    public:
        StartHerePage(wxEvtHandler* owner, wxWindow* parent);
        virtual ~StartHerePage();
        bool LinkClicked(const wxHtmlLinkInfo& link);
        void SetPageContent(const wxString& buffer); // set the HTML content
        virtual bool VisibleToTree() const { return false; }
        void Reload();
    protected:
        wxEvtHandler* m_pOwner;
        wxHtmlWindow* m_pWin;
        wxString m_OriginalPageContent;
    private:
        DECLARE_EVENT_TABLE()
};

#endif // STARTHEREPAGE_H