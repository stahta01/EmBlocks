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
#endif

#include "genericmultilinenotesdlg.h"
#include <wx/textctrl.h>

BEGIN_EVENT_TABLE(GenericMultiLineNotesDlg, wxScrollingDialog)
    //
END_EVENT_TABLE()

GenericMultiLineNotesDlg::GenericMultiLineNotesDlg(wxWindow* parent, const wxString& caption, const wxString& notes, bool readOnly)
    : m_Notes(notes),
    m_ReadOnly(readOnly)
{
    //ctor
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgGenericMultiLineNotes"),_T("wxScrollingDialog"));
    SetTitle(caption);

    XRCCTRL(*this, "txtNotes", wxTextCtrl)->SetValue(m_Notes);
    if (m_ReadOnly)
    {
        XRCCTRL(*this, "txtNotes", wxTextCtrl)->Disable();
        if (wxWindow* win = FindWindowById(wxID_CANCEL, this))
        {
            win->Enable(false);
        }
    }
}

GenericMultiLineNotesDlg::~GenericMultiLineNotesDlg()
{
    //dtor
}

void GenericMultiLineNotesDlg::EndModal(int retCode)
{
    if (retCode == wxID_OK && !m_ReadOnly)
    {
        m_Notes = XRCCTRL(*this, "txtNotes", wxTextCtrl)->GetValue();
    }
    wxScrollingDialog::EndModal(retCode);
}
