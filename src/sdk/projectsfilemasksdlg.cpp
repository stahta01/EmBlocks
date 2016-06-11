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
    #include <wx/button.h>
    #include <wx/textctrl.h>
    #include <wx/msgdlg.h>
    #include <wx/listbox.h>
    #include "globals.h"
#endif

#include <wx/textdlg.h>
#include "projectsfilemasksdlg.h"

BEGIN_EVENT_TABLE(ProjectsFileMasksDlg, wxScrollingDialog)
    EVT_UPDATE_UI( -1, ProjectsFileMasksDlg::OnUpdateUI)
    EVT_BUTTON(XRCID("btnAdd"), ProjectsFileMasksDlg::OnAdd)
    EVT_BUTTON(XRCID("btnEdit"), ProjectsFileMasksDlg::OnEdit)
    EVT_BUTTON(XRCID("btnDelete"), ProjectsFileMasksDlg::OnDelete)
    EVT_BUTTON(XRCID("btnSetDefault"), ProjectsFileMasksDlg::OnSetDefault)
    EVT_LISTBOX(XRCID("lstCategories"), ProjectsFileMasksDlg::OnListChanged)
END_EVENT_TABLE()

ProjectsFileMasksDlg::ProjectsFileMasksDlg(wxWindow* parent, FilesGroupsAndMasks* fgam) :
    m_FileGroupsAndMasksCopy(*fgam), // store a local copy, so if we press "Cancel", we can revert to the original...
    m_pFileGroupsAndMasks(fgam),
    m_LastListSelection(0)
{
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgProjectsFileMasks"),_T("wxScrollingDialog"));

    RebuildList();
}

ProjectsFileMasksDlg::~ProjectsFileMasksDlg()
{
    //dtor
}

void ProjectsFileMasksDlg::RebuildList()
{
    wxListBox* pList = XRCCTRL(*this, "lstCategories", wxListBox);
    pList->Clear();
    for (unsigned int i = 0; i < m_FileGroupsAndMasksCopy.GetGroupsCount(); ++i)
        pList->Append(m_FileGroupsAndMasksCopy.GetGroupName(i));

    if (pList->GetCount() != 0)
    {
        pList->SetSelection(m_LastListSelection);
        ListChange();
    }
}

void ProjectsFileMasksDlg::ListChange()
{
    wxTextCtrl* pText = XRCCTRL(*this, "txtFileMasks", wxTextCtrl);
    int sel = XRCCTRL(*this, "lstCategories", wxListBox)->GetSelection();

    if (sel != m_LastListSelection)
    {
        // switching group; see if the user changed the masks...
        if (pText->GetValue() != m_FileGroupsAndMasksCopy.GetFileMasks(m_LastListSelection))
            m_FileGroupsAndMasksCopy.SetFileMasks(m_LastListSelection, pText->GetValue());
    }

    pText->SetValue(m_FileGroupsAndMasksCopy.GetFileMasks(sel));
    m_LastListSelection = sel;
}

void ProjectsFileMasksDlg::OnUpdateUI(wxUpdateUIEvent& /*event*/)
{
    int sel = XRCCTRL(*this, "lstCategories", wxListBox)->GetSelection();

    XRCCTRL(*this, "btnEdit", wxButton)->Enable(sel >= 0);
    XRCCTRL(*this, "btnDelete", wxButton)->Enable(sel >= 0);
}

void ProjectsFileMasksDlg::OnAdd(wxCommandEvent& /*event*/)
{
    wxString groupName = wxGetTextFromUser(_("Enter the new group name:"),
                                            _("New group"));
    if (groupName.IsEmpty())
        return;
    m_FileGroupsAndMasksCopy.AddGroup(groupName);
    wxListBox* pList = XRCCTRL(*this, "lstCategories", wxListBox);
    pList->Append(groupName);
    pList->SetSelection(pList->GetCount() - 1);
    ListChange();
    XRCCTRL(*this, "txtFileMasks", wxTextCtrl)->SetFocus();
}

void ProjectsFileMasksDlg::OnEdit(wxCommandEvent& /*event*/)
{
    wxListBox* pList = XRCCTRL(*this, "lstCategories", wxListBox);
    wxString oldName = pList->GetStringSelection();
    wxString groupName = wxGetTextFromUser(_("Rename the group:"),
                                            _("Edit group"), oldName);
    if (!groupName.IsEmpty() && groupName != oldName)
    {
        m_FileGroupsAndMasksCopy.RenameGroup(pList->GetSelection(), groupName);
        pList->SetString(pList->GetSelection(), groupName);
    }
}

void ProjectsFileMasksDlg::OnDelete(wxCommandEvent& /*event*/)
{
    wxListBox* pList = XRCCTRL(*this, "lstCategories", wxListBox);
    wxString name = pList->GetStringSelection();
    wxString caption;
    caption.Printf(_("Are you sure you want to delete the group \"%s\"?"), name.c_str());
    if (cbMessageBox(caption, _("Confirmation"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION, this) == wxID_NO)
        return;
    m_FileGroupsAndMasksCopy.DeleteGroup(pList->GetSelection());
    RebuildList();
}

void ProjectsFileMasksDlg::OnSetDefault(wxCommandEvent& /*event*/)
{
    m_FileGroupsAndMasksCopy.SetDefault();
    RebuildList();
}

void ProjectsFileMasksDlg::OnListChanged(wxCommandEvent& /*event*/)
{
    ListChange();
}

void ProjectsFileMasksDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        wxTextCtrl* pText = XRCCTRL(*this, "txtFileMasks", wxTextCtrl);
        if (pText->GetValue() != m_FileGroupsAndMasksCopy.GetFileMasks(m_LastListSelection))
            m_FileGroupsAndMasksCopy.SetFileMasks(m_LastListSelection, pText->GetValue());

        m_pFileGroupsAndMasks->CopyFrom(m_FileGroupsAndMasksCopy);
    }

    wxScrollingDialog::EndModal(retCode);
}
