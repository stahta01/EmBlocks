/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 4 $
 * $Id: selectincludefile.cpp 4 2013-11-02 15:53:52Z gerard $
 * $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/codecompletion/selectincludefile.cpp $
 */

#include "selectincludefile.h"

#include <wx/xrc/xmlres.h>
#include <wx/arrstr.h>

BEGIN_EVENT_TABLE(SelectIncludeFile,wxScrollingDialog)
    EVT_LISTBOX_DCLICK(XRCID("ID_LBX_INCLUDE_FILES"),SelectIncludeFile::OnOk)
    EVT_BUTTON(XRCID("ID_BTN_OK"),SelectIncludeFile::OnOk)
    EVT_BUTTON(XRCID("ID_BTN_CANCEL"),SelectIncludeFile::OnCancel)
END_EVENT_TABLE()

SelectIncludeFile::SelectIncludeFile(wxWindow* parent,wxWindowID id)
{
    wxXmlResource::Get()->LoadObject(this,parent,_T("SelectIncludeFile"),_T("wxScrollingDialog"));
    LstIncludeFiles = XRCCTRL(*this,"ID_LBX_INCLUDE_FILES",wxListBox);
}

SelectIncludeFile::~SelectIncludeFile()
{
}

void SelectIncludeFile::OnOk(wxCommandEvent& event)
{
    int pos = LstIncludeFiles->GetSelection();
    if (pos != wxNOT_FOUND)
        m_SelectedIncludeFile = LstIncludeFiles->GetString(pos);
    else
        m_SelectedIncludeFile.Clear();

    EndModal(wxID_OK);
}

void SelectIncludeFile::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void SelectIncludeFile::AddListEntries(const wxArrayString& IncludeFiles)
{
    LstIncludeFiles->InsertItems(IncludeFiles, 0);
    LstIncludeFiles->SetSelection(0);
}