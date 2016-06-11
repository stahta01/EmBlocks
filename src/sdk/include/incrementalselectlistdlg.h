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
#ifndef INCREMENTALSELECTLISTDLG_H
#define INCREMENTALSELECTLISTDLG_H

#include "settings.h"
#include "scrollingdialog.h"
#include <wx/textctrl.h>
#include <wx/listbox.h>

class myHandler; // forward decl
class IncrementalSelectIterator; // forward decl


class DLLIMPORT IncrementalSelectListDlg : public wxScrollingDialog
{
    public:
        IncrementalSelectListDlg(wxWindow* parent,
                                 const IncrementalSelectIterator& iterator,
                                 const wxString& caption = wxEmptyString,
                                 const wxString& message = wxEmptyString);
        virtual ~IncrementalSelectListDlg();
        wxString GetStringSelection();
        long GetSelection();
    protected:
        void FillList();
        void OnSearch(wxCommandEvent& event);
        void OnSelect(wxCommandEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        myHandler* m_pMyEvtHandler;
        wxListBox* m_List;
        wxTextCtrl* m_Text;
        const IncrementalSelectIterator &m_Iterator;
    private:
        DECLARE_EVENT_TABLE();
};

class myHandler : public wxEvtHandler
{
    public:
        myHandler(wxScrollingDialog* parent, wxTextCtrl* text, wxListBox* list)
            : m_pParent(parent),
            m_pList(list),
            m_pText(text)
        {
            Connect( text->GetId(),  -1, wxEVT_KEY_DOWN,
                          (wxObjectEventFunction) (wxEventFunction) (wxCharEventFunction)
                          &myHandler::OnKeyDown );
            Connect( list->GetId(),  -1, wxEVT_KEY_DOWN,
                          (wxObjectEventFunction) (wxEventFunction) (wxCharEventFunction)
                          &myHandler::OnKeyDown );
        }
        void OnKeyDown(wxKeyEvent& event);
    private:
        wxScrollingDialog* m_pParent;
        wxListBox* m_pList;
        wxTextCtrl* m_pText;
        DECLARE_EVENT_TABLE();
};


class DLLIMPORT IncrementalSelectIterator
{
    public:
        virtual ~IncrementalSelectIterator() {}

        virtual long GetCount() const = 0;
        virtual wxString GetItem(long index) const = 0;
};

class DLLIMPORT IncrementalSelectIteratorStringArray : public IncrementalSelectIterator
{
    public:
        IncrementalSelectIteratorStringArray(const wxArrayString& array) : m_Array(array)
        {
        }

        virtual long GetCount() const { return m_Array.GetCount(); }
        virtual wxString GetItem(long index) const { return m_Array[index]; }
    private:
        const wxArrayString& m_Array;
};

#endif // INCREMENTALSELECTLISTDLG_H
