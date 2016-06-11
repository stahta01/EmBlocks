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
#include "virtualbuildtargetsdlg.h"

#ifndef CB_PRECOMP
    #include "cbproject.h"
    #include "globals.h"
    #include "wx/textdlg.h"
    #include "wx/utils.h"
#endif

//(*InternalHeaders(VirtualBuildTargetsDlg)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(VirtualBuildTargetsDlg)
//*)

BEGIN_EVENT_TABLE(VirtualBuildTargetsDlg,wxScrollingDialog)
    //(*EventTable(VirtualBuildTargetsDlg)
    //*)
    EVT_UPDATE_UI(-1, VirtualBuildTargetsDlg::OnUpdateUI)
END_EVENT_TABLE()

VirtualBuildTargetsDlg::VirtualBuildTargetsDlg(wxWindow* parent,wxWindowID /*id*/, cbProject* project) :
    m_pProject(project)
{
    //(*Initialize(VirtualBuildTargetsDlg)
    wxXmlResource::Get()->LoadObject(this,parent,_T("VirtualBuildTargetsDlg"),_T("wxScrollingDialog"));
    lstAliases = (wxListBox*)FindWindow(XRCID("ID_LST_ALIASES"));
    btnAdd = (wxButton*)FindWindow(XRCID("ID_BTN_ADD"));
    btnEdit = (wxButton*)FindWindow(XRCID("ID_BTN_EDIT"));
    btnRemove = (wxButton*)FindWindow(XRCID("ID_BTN_REMOVE"));
    lstTargets = (wxCheckListBox*)FindWindow(XRCID("ID_LST_TARGETS"));

    Connect(XRCID("ID_LST_ALIASES"),wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&VirtualBuildTargetsDlg::OnAliasesSelect);
    Connect(XRCID("ID_BTN_ADD"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VirtualBuildTargetsDlg::OnAddClick);
    Connect(XRCID("ID_BTN_EDIT"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VirtualBuildTargetsDlg::OnEditClick);
    Connect(XRCID("ID_BTN_REMOVE"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&VirtualBuildTargetsDlg::OnRemoveClick);
    Connect(XRCID("ID_LST_TARGETS"),wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,(wxObjectEventFunction)&VirtualBuildTargetsDlg::OnTargetsToggled);
    //*)

    // fill aliases
    wxArrayString virtuals = m_pProject->GetVirtualBuildTargets();
    lstAliases->Set(virtuals);
    lstAliases->SetSelection(0);

    // fill build targets
    for (int i = 0; i < m_pProject->GetBuildTargetsCount(); ++i)
        lstTargets->Append(m_pProject->GetBuildTarget(i)->GetTitle());
    CheckTargets();
}

VirtualBuildTargetsDlg::~VirtualBuildTargetsDlg()
{
    //(*Destroy(VirtualBuildTargetsDlg)
    //*)
}

void VirtualBuildTargetsDlg::SetVirtualTarget(const wxString& targetName)
{
    if (!lstTargets->IsEnabled() || lstTargets->IsEmpty())
        return;

    wxArrayString checked;
    for (int i = 0; i < (int)lstTargets->GetCount(); ++i)
    {
        if (lstTargets->IsChecked(i))
            checked.Add(lstTargets->GetString(i));
    }

    if (checked.GetCount() == 0)
    {
        cbMessageBox(_("This virtual build target is invalid.\n"
                       "A virtual target must have at least one active target.\n"
                       "Did you want to remove the virtual build target?"), _("Error"), wxICON_ERROR, this);
    }
    else if (checked.GetCount() > 0)
    {
        if ( !m_pProject->DefineVirtualBuildTarget(targetName, checked) )
            cbMessageBox(_("Failed to setup this virtual build target.\n"
                           "Check the debug log for more info..."), _("Error"), wxICON_ERROR, this);
    }
}

void VirtualBuildTargetsDlg::CheckTargets()
{
    const wxArrayString& group = m_pProject->GetVirtualBuildTargetGroup( lstAliases->GetStringSelection() );
    for (int i = 0; i < m_pProject->GetBuildTargetsCount(); ++i)
    {
        wxString tgtName = m_pProject->GetBuildTarget(i)->GetTitle();
        bool check = group.Index(tgtName) != wxNOT_FOUND;
        lstTargets->Check(i, check);
    }
}

void VirtualBuildTargetsDlg::OnUpdateUI(wxUpdateUIEvent& /*event*/)
{
    bool hasSel = lstAliases->GetSelection() != -1;
    btnEdit->Enable(hasSel);
    btnRemove->Enable(hasSel);
    lstTargets->Enable(hasSel);
}

void VirtualBuildTargetsDlg::OnAddClick(wxCommandEvent& /*event*/)
{
    wxString targetName = wxGetTextFromUser(_("Enter the new virtual build target name:"),
                                            _("New virtual build target"));
    if (targetName.IsEmpty())
        return;

    if (lstAliases->FindString(targetName) != wxNOT_FOUND)
    {
        cbMessageBox(_("A virtual build target with this name already exists in this project!"),
                        _("Error"),
                        wxOK | wxCENTRE | wxICON_ERROR, this);
        return;
    }

    if (m_pProject->GetBuildTarget(targetName))
    {
        cbMessageBox(_("A real build target with this name already exists in this project!"),
                        _("Error"),
                        wxOK | wxCENTRE | wxICON_ERROR, this);
        return;
    }

    // add it with an empty group
    lstAliases->Append(targetName);
    lstAliases->SetSelection(lstAliases->GetCount() - 1);
    CheckTargets();
}

void VirtualBuildTargetsDlg::OnEditClick(wxCommandEvent& /*event*/)
{
    wxString targetName = wxGetTextFromUser(_("Enter the new virtual build target name:"),
                                            _("Edit virtual build target"),
                                            lstAliases->GetStringSelection());

    // is name unchanged, or user cancelled?
    if (targetName.IsEmpty() || targetName == lstAliases->GetStringSelection())
        return;

    if (lstAliases->FindString(targetName) != wxNOT_FOUND)
    {
        cbMessageBox(_("A virtual build target with this name already exists in this project!"),
                        _("Error"),
                        wxOK | wxCENTRE | wxICON_ERROR, this);
        return;
    }

    if (m_pProject->GetBuildTarget(targetName))
    {
        cbMessageBox(_("A real build target with this name already exists in this project!"),
                        _("Error"),
                        wxOK | wxCENTRE | wxICON_ERROR, this);
        return;
    }

    m_pProject->RemoveVirtualBuildTarget(lstAliases->GetStringSelection());
    lstAliases->SetString(lstAliases->GetSelection(), targetName);
    SetVirtualTarget(targetName);
}

void VirtualBuildTargetsDlg::OnRemoveClick(wxCommandEvent& /*event*/)
{
    if (cbMessageBox(_("Are you sure you want to remove this virtual build target?"), _("Confirmation"), wxYES_NO | wxICON_QUESTION, this) == wxID_NO)
        return;
    m_pProject->RemoveVirtualBuildTarget(lstAliases->GetStringSelection());
    lstAliases->Delete(lstAliases->GetSelection());
    lstAliases->SetSelection(0);
    CheckTargets();
}

void VirtualBuildTargetsDlg::OnAliasesSelect(wxCommandEvent& /*event*/)
{
    CheckTargets();
}

void VirtualBuildTargetsDlg::OnTargetsToggled(wxCommandEvent& /*event*/)
{
    SetVirtualTarget(lstAliases->GetStringSelection());
}
