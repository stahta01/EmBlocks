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
    #include "cbproject.h"
    #include "projectbuildtarget.h"
    #include <wx/xrc/xmlres.h>
    #include <wx/button.h>
    #include <wx/listbox.h>
    #include "globals.h"
#endif

#include "externaldepsdlg.h"
#include <wx/msgdlg.h>
#include "editpathdlg.h"

BEGIN_EVENT_TABLE(ExternalDepsDlg, wxScrollingDialog)
    EVT_UPDATE_UI(-1, ExternalDepsDlg::OnUpdateUI)
    EVT_LISTBOX_DCLICK(XRCID("lstAdditionalFiles"), ExternalDepsDlg::OnEditAdditional)
    EVT_LISTBOX_DCLICK(XRCID("lstExternalFiles"), ExternalDepsDlg::OnEditExternal)
    EVT_BUTTON(XRCID("btnAddAdditional"), ExternalDepsDlg::OnAddAdditional)
    EVT_BUTTON(XRCID("btnEditAdditional"), ExternalDepsDlg::OnEditAdditional)
    EVT_BUTTON(XRCID("btnDelAdditional"), ExternalDepsDlg::OnDelAdditional)
    EVT_BUTTON(XRCID("btnAddExternal"), ExternalDepsDlg::OnAddExternal)
    EVT_BUTTON(XRCID("btnEditExternal"), ExternalDepsDlg::OnEditExternal)
    EVT_BUTTON(XRCID("btnDelExternal"), ExternalDepsDlg::OnDelExternal)
END_EVENT_TABLE()

ExternalDepsDlg::ExternalDepsDlg(wxWindow* parent, cbProject* project, ProjectBuildTarget* target)
    : m_pProject(project),
    m_pTarget(target)
{
	//ctor
	wxXmlResource::Get()->LoadObject(this, parent, _T("dlgExternalDeps"),_T("wxScrollingDialog"));
	FillAdditional();
	FillExternal();
}

ExternalDepsDlg::~ExternalDepsDlg()
{
	//dtor
}

void ExternalDepsDlg::FillAdditional()
{
	wxListBox* lst = XRCCTRL(*this, "lstAdditionalFiles", wxListBox);
	lst->Clear();
    wxArrayString array = GetArrayFromString(m_pTarget->GetAdditionalOutputFiles());
    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
    	lst->Append(array[i]);
    }
}

void ExternalDepsDlg::FillExternal()
{
	wxListBox* lst = XRCCTRL(*this, "lstExternalFiles", wxListBox);
	lst->Clear();
    wxArrayString array = GetArrayFromString(m_pTarget->GetExternalDeps());
    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
    	lst->Append(array[i]);
    }
}

void ExternalDepsDlg::EndModal(int retCode)
{
	wxString deps;
	wxListBox* lst = XRCCTRL(*this, "lstExternalFiles", wxListBox);
    for (unsigned int i = 0; i < (unsigned int)lst->GetCount(); ++i)
    {
    	deps << lst->GetString(i) << _T(';');
    }
    m_pTarget->SetExternalDeps(deps);

	wxString files;
	lst = XRCCTRL(*this, "lstAdditionalFiles", wxListBox);
    for (unsigned int i = 0; i < (unsigned int)lst->GetCount(); ++i)
    {
    	files << lst->GetString(i) << _T(';');
    }
    m_pTarget->SetAdditionalOutputFiles(files);

	return wxScrollingDialog::EndModal(retCode);
}

//void ExternalDepsDlg::DoAdd(const wxString& listbox, const wxString& message)
#define DoAdd(listbox,message)                              \
{                                                           \
	wxListBox* lst = XRCCTRL(*this, listbox, wxListBox);    \
    EditPathDlg dlg(this,                                   \
                    m_pProject->GetBasePath(),              \
                    m_pProject->GetBasePath(),              \
                    message,                                \
                    wxEmptyString,                          \
                    false);                                 \
    PlaceWindow(&dlg);                                      \
    if (dlg.ShowModal() == wxID_OK)                         \
        lst->Append(dlg.GetPath());                         \
}

//void ExternalDepsDlg::DoEdit(const wxString& listbox, const wxString& message)
#define DoEdit(listbox,message)                             \
{                                                           \
	wxListBox* lst = XRCCTRL(*this, listbox, wxListBox);    \
	int sel = lst->GetSelection();                          \
	if (sel == -1)                                          \
        return;                                             \
    EditPathDlg dlg(this,                                   \
                    lst->GetStringSelection(),              \
                    m_pProject->GetBasePath(),              \
                    message,                                \
                    wxEmptyString,                          \
                    false);                                 \
    PlaceWindow(&dlg);                                      \
    if (dlg.ShowModal() == wxID_OK)                         \
        lst->SetString(sel, dlg.GetPath());                 \
}

//void ExternalDepsDlg::DoDel(const wxString& listbox)
#define DoDel(listbox)                                      \
{                                                           \
	wxListBox* lst = XRCCTRL(*this, listbox, wxListBox);    \
	int sel = lst->GetSelection();                          \
	if (sel == -1)                                          \
        return;                                             \
    if (cbMessageBox(_("Are you sure you want to remove this file?"), _("Remove file"), wxYES_NO | wxNO_DEFAULT, this) == wxID_NO)   \
        return;                                             \
    lst->Delete(sel);                                       \
}

void ExternalDepsDlg::OnAddAdditional(wxCommandEvent& /*event*/)
{
	DoAdd("lstAdditionalFiles", _("Add additional output file"));
}

void ExternalDepsDlg::OnEditAdditional(wxCommandEvent& /*event*/)
{
	DoEdit("lstAdditionalFiles", _("Edit additional output file"));
}

void ExternalDepsDlg::OnDelAdditional(wxCommandEvent& /*event*/)
{
	DoDel("lstAdditionalFiles");
}

void ExternalDepsDlg::OnAddExternal(wxCommandEvent& /*event*/)
{
	DoAdd("lstExternalFiles", _("Add external dependency file"));
}

void ExternalDepsDlg::OnEditExternal(wxCommandEvent& /*event*/)
{
	DoEdit("lstExternalFiles", _("Edit external dependency file"));
}

void ExternalDepsDlg::OnDelExternal(wxCommandEvent& /*event*/)
{
	DoDel("lstExternalFiles");
}

void ExternalDepsDlg::OnUpdateUI(wxUpdateUIEvent& /*event*/)
{
	int selAdd = XRCCTRL(*this, "lstAdditionalFiles", wxListBox)->GetSelection();
	int selExt = XRCCTRL(*this, "lstExternalFiles", wxListBox)->GetSelection();

	XRCCTRL(*this, "btnEditAdditional", wxButton)->Enable(selAdd != -1);
	XRCCTRL(*this, "btnDelAdditional", wxButton)->Enable(selAdd != -1);
	XRCCTRL(*this, "btnEditExternal", wxButton)->Enable(selExt != -1);
	XRCCTRL(*this, "btnDelExternal", wxButton)->Enable(selExt != -1);
}
