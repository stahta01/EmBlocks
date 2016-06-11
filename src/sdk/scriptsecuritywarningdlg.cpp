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
#include "scriptsecuritywarningdlg.h"

#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>

ScriptSecurityWarningDlg::ScriptSecurityWarningDlg(wxWindow* parent, const wxString& operation, const wxString& command)
{
    //ctor
    wxXmlResource::Get()->LoadObject(this, parent, _T("ScriptingSecurityDlg"),_T("wxScrollingDialog"));

    wxColour c = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    XRCCTRL(*this, "txtCommand", wxTextCtrl)->SetBackgroundColour(c);

    XRCCTRL(*this, "lblOperation", wxStaticText)->SetLabel(_("Operation: ") + operation);
    XRCCTRL(*this, "txtCommand", wxTextCtrl)->SetValue(command);
}

ScriptSecurityWarningDlg::~ScriptSecurityWarningDlg()
{
    //dtor
}

ScriptSecurityResponse ScriptSecurityWarningDlg::GetResponse()
{
    return (ScriptSecurityResponse)XRCCTRL(*this, "cmbAnswer", wxComboBox)->GetSelection();
}

void ScriptSecurityWarningDlg::EndModal(int retCode)
{
    wxScrollingDialog::EndModal(retCode);
}
