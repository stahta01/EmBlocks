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
    #include <wx/xrc/xmlres.h>
    #include <wx/intl.h>
    #include <wx/textctrl.h>
    #include <wx/stattext.h>
    #include "manager.h"
    #include <wx/listbox.h>
#endif

#include "incrementalselectlistdlg.h"


BEGIN_EVENT_TABLE(myHandler, wxEvtHandler)
END_EVENT_TABLE()

void myHandler::OnKeyDown(wxKeyEvent& event)
{
    //Manager::Get()->GetLogManager()->Log(mltDevDebug, "OnKeyDown");
    size_t sel = 0;
    switch (event.GetKeyCode())
    {
        case WXK_RETURN:
            m_pParent->EndModal(wxID_OK);
            break;

        case WXK_ESCAPE:
            m_pParent->EndModal(wxID_CANCEL);
            break;

        case WXK_UP:
            sel = m_pList->GetSelection() - 1;
            m_pList->SetSelection(sel == (size_t) -1 ? 0 : sel);
            break;

        case WXK_DOWN:
            m_pList->SetSelection(m_pList->GetSelection() + 1);
            break;

        case WXK_PAGEUP:
            sel = m_pList->GetSelection() - 10;
            m_pList->SetSelection( sel > m_pList->GetCount() ? 0 : sel );
            break;

        case WXK_PAGEDOWN:
            sel = m_pList->GetSelection() + 10;
            m_pList->SetSelection( sel >= m_pList->GetCount() ? m_pList->GetCount() - 1 : sel );
            break;

        case WXK_HOME:
            m_pList->SetSelection(0);
            break;

        case WXK_END:
            m_pList->SetSelection( m_pList->GetCount() - 1 );
            event.Skip();
            break;

        default:
            event.Skip();
            break;
    }
}

BEGIN_EVENT_TABLE(IncrementalSelectListDlg, wxScrollingDialog)
    EVT_TEXT(XRCID("txtSearch"), IncrementalSelectListDlg::OnSearch)
    EVT_LISTBOX_DCLICK(XRCID("lstItems"), IncrementalSelectListDlg::OnSelect)
END_EVENT_TABLE()

IncrementalSelectListDlg::IncrementalSelectListDlg(wxWindow* parent, const IncrementalSelectIterator& iterator,
                                                   const wxString& caption, const wxString& message)
    : m_pMyEvtHandler(0L),
    m_List(0L),
    m_Text(0L),
    m_Iterator(iterator)
{
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgIncrementalSelectList"),_T("wxScrollingDialog"));
    if (!caption.IsEmpty())
        SetTitle(caption);
    if (!message.IsEmpty())
        XRCCTRL(*this, "lblMessage", wxStaticText)->SetLabel(message);

    m_Text = XRCCTRL(*this, "txtSearch", wxTextCtrl);
    m_List = XRCCTRL(*this, "lstItems", wxListBox);

    myHandler* m_pMyEvtHandler = new myHandler(this, m_Text, m_List);
    m_Text->SetNextHandler(m_pMyEvtHandler);
    m_List->SetNextHandler(m_pMyEvtHandler);

    FillList();
}

IncrementalSelectListDlg::~IncrementalSelectListDlg()
{
    m_Text->SetNextHandler(0L);
    m_List->SetNextHandler(0L);

    delete m_pMyEvtHandler;
}

wxString IncrementalSelectListDlg::GetStringSelection()
{
    return m_List->GetStringSelection();
}

long IncrementalSelectListDlg::GetSelection()
{
    int selection = m_List->GetSelection();
    if (selection == wxNOT_FOUND)
        return wxNOT_FOUND;

    return reinterpret_cast<long>(m_List->GetClientData(selection));
}

void IncrementalSelectListDlg::FillList()
{
    Freeze();

    // We put a star before and after pattern to find search expression everywhere in path
    wxString search(wxT("*") + m_Text->GetValue().Lower() + wxT("*"));

    wxArrayString result;
    wxArrayLong indexes;

    m_List->Clear();
    for (int i = 0; i < m_Iterator.GetCount(); ++i)
    {
        wxString const &item = m_Iterator.GetItem(i);
        // 2 for before and after stars =~ empty string
        if ((search.Length()==2) || item.Lower().Matches(search.c_str()))
        {
            result.Add(item);
            indexes.Add(i);
        }
    }
    m_List->Set(result, reinterpret_cast<void**>(&indexes[0]));
    if (m_List->GetCount())
        m_List->SetSelection(0);

    Thaw();
}

// events

void IncrementalSelectListDlg::OnSearch(wxCommandEvent& /*event*/)
{
    FillList();
}

void IncrementalSelectListDlg::OnSelect(wxCommandEvent& /*event*/)
{
    EndModal(wxID_OK);
}
