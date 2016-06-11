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
    #include <wx/intl.h>
    #include <wx/xrc/xmlres.h>
    #include <wx/button.h>
    #include <wx/msgdlg.h>
    #include <wx/listbox.h>
    #include "globals.h"
#endif

#include <wx/filedlg.h>
#include "editarrayfiledlg.h"
#include "filefilters.h"

BEGIN_EVENT_TABLE(EditArrayFileDlg, wxScrollingDialog)
	EVT_LISTBOX_DCLICK(XRCID("lstItems"), EditArrayFileDlg::OnEdit)
	EVT_BUTTON(XRCID("btnAdd"), EditArrayFileDlg::OnAdd)
	EVT_BUTTON(XRCID("btnEdit"), EditArrayFileDlg::OnEdit)
	EVT_BUTTON(XRCID("btnDelete"), EditArrayFileDlg::OnDelete)
	EVT_UPDATE_UI(-1, EditArrayFileDlg::OnUpdateUI)
END_EVENT_TABLE()

EditArrayFileDlg::EditArrayFileDlg(wxWindow* parent, wxArrayString& array, bool useRelativePaths, const wxString& basePath)
	: m_Array(array),
	m_UseRelativePaths(useRelativePaths),
	m_BasePath(basePath)
{
	//ctor
	wxXmlResource::Get()->LoadObject(this, parent, _T("dlgEditArrayString"),_T("wxScrollingDialog"));

	wxListBox* list = XRCCTRL(*this, "lstItems", wxListBox);
	list->Clear();
	for (unsigned int i = 0; i < m_Array.GetCount(); ++i)
	{
        wxFileName fname;
        fname.Assign(m_Array[i]);
        if (!m_UseRelativePaths && fname.IsRelative())
            fname.Normalize(wxPATH_NORM_ALL & ~wxPATH_NORM_CASE, m_BasePath);
        else if (m_UseRelativePaths && fname.IsAbsolute())
            fname.MakeRelativeTo(m_BasePath);
        m_Array[i] = fname.GetFullPath();
        list->Append(m_Array[i]);
	}
}

EditArrayFileDlg::~EditArrayFileDlg()
{
	//dtor
}

void EditArrayFileDlg::EndModal(int retCode)
{
	if (retCode == wxID_OK)
	{
		wxListBox* list = XRCCTRL(*this, "lstItems", wxListBox);
		m_Array.Clear();
		for (int i = 0; i < (int)list->GetCount(); ++i)
		{
			m_Array.Add(list->GetString(i));
		}
	}
	wxScrollingDialog::EndModal(retCode);
}

// events

void EditArrayFileDlg::OnAdd(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog dlg(this,
                    _("Select file"),
                    m_BasePath,
                    _T(""),
                    FileFilters::GetFilterAll(),
                    wxFD_OPEN | compatibility::wxHideReadonly);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return;
    wxFileName fname;
    fname.Assign(dlg.GetPath());
    if (m_UseRelativePaths)
        fname.MakeRelativeTo(m_BasePath);
    XRCCTRL(*this, "lstItems", wxListBox)->Append(fname.GetFullPath());
}

void EditArrayFileDlg::OnEdit(wxCommandEvent& WXUNUSED(event))
{
	wxListBox* list = XRCCTRL(*this, "lstItems", wxListBox);
    wxFileDialog dlg(this,
                    _("Select file"),
                    m_BasePath,
                    list->GetStringSelection(),
                    FileFilters::GetFilterAll(),
                    wxFD_OPEN | compatibility::wxHideReadonly);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return;
    wxFileName fname;
    fname.Assign(dlg.GetPath());
    if (m_UseRelativePaths)
        fname.MakeRelativeTo(m_BasePath);
	list->SetString(list->GetSelection(), fname.GetFullPath());
}

void EditArrayFileDlg::OnDelete(wxCommandEvent& WXUNUSED(event))
{
	if (cbMessageBox(_("Delete this item?"), _("Confirm"), wxYES_NO, this) == wxID_YES)
	{
		wxListBox* list = XRCCTRL(*this, "lstItems", wxListBox);
		list->Delete(list->GetSelection());
	}
}

void EditArrayFileDlg::OnUpdateUI(wxUpdateUIEvent& WXUNUSED(event))
{
	bool en = XRCCTRL(*this, "lstItems", wxListBox)->GetSelection() != -1;
	XRCCTRL(*this, "btnEdit", wxButton)->Enable(en);
	XRCCTRL(*this, "btnDelete", wxButton)->Enable(en);
}
