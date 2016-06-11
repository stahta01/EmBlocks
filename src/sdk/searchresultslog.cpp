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

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/arrstr.h>
    #include <wx/filename.h>
    #include <wx/listctrl.h>
    #include "manager.h"
    #include "editormanager.h"
    #include "cbeditor.h"
#endif
#include "cbstyledtextctrl.h"

#include "searchresultslog.h"

namespace
{
	const int ID_List = wxNewId();
};

BEGIN_EVENT_TABLE(SearchResultsLog, wxEvtHandler)
//
END_EVENT_TABLE()

SearchResultsLog::SearchResultsLog(const wxArrayString& titles, wxArrayInt& widths)
    : ListCtrlLogger(titles, widths)
{
	//ctor
}

SearchResultsLog::~SearchResultsLog()
{
	//dtor
}

wxWindow* SearchResultsLog::CreateControl(wxWindow* parent)
{
	ListCtrlLogger::CreateControl(parent);
    control->SetId(ID_List);
    Connect(ID_List, -1, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
            &SearchResultsLog::OnDoubleClick);
	control->PushEventHandler(this);
	return control;
};

void SearchResultsLog::FocusEntry(size_t index)
{
    if (index >= 0 && index < (size_t)control->GetItemCount())
    {
        control->SetItemState(index, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED, wxLIST_STATE_FOCUSED | wxLIST_STATE_SELECTED);
        control->EnsureVisible(index);
        SyncEditor(index);
    }
}

void SearchResultsLog::SyncEditor(int selIndex)
{
    wxFileName filename(control->GetItemText(selIndex));
    wxString file;
    if (!filename.IsAbsolute())
        filename.MakeAbsolute(m_Base);
    file = filename.GetFullPath();

    wxListItem li;
    li.m_itemId = selIndex;
    li.m_col = 1;
    li.m_mask = wxLIST_MASK_TEXT;
    control->GetItem(li);
    long line = 0;
    li.m_text.ToLong(&line);
    cbEditor* ed = Manager::Get()->GetEditorManager()->Open(file);
    if (!line || !ed)
        return;

    line -= 1;
    ed->Activate();
    ed->GotoLine(line);

    // Save the new cursor pos. if there are plugins for this.
    CodeBlocksEvent evt(cbEVT_CURSOR_POS_SAVE);
    Manager::Get()->GetPluginManager()->NotifyPlugins(evt);

    if (cbStyledTextCtrl* control = ed->GetControl()) {
        control->EnsureVisible(line);
    }
}

void SearchResultsLog::OnDoubleClick(wxCommandEvent& /*event*/)
{
    // go to the relevant file/line
    if (control->GetSelectedItemCount() == 0)
        return;

    // find selected item index
    int index = control->GetNextItem(-1,
                                     wxLIST_NEXT_ALL,
                                     wxLIST_STATE_SELECTED);

    SyncEditor(index);
} // end of OnDoubleClick
