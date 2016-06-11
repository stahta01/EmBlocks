/*
 * This file is part of the code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
*/
/*
    Copyright (C) Em::Blocks 2011-2013

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


    @version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:

 */

#include "sdk.h"
#ifndef CB_PRECOMP
#include <wx/intl.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>
#include <wx/string.h>
#include <wx/xrc/xmlres.h>
#include "cbeditor.h"
#include "configmanager.h"
#include "editormanager.h"
#include "manager.h"
#endif
#include "cbstyledtextctrl.h"

#include "printdlg.h"

PrintDialog::PrintDialog(wxWindow* parent)
{
	//ctor
	wxXmlResource::Get()->LoadObject(this, parent, _T("dlgPrint"),_T("wxScrollingDialog"));

	cbEditor* ed = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
	if (ed)
	{
        bool hasSel = ed->GetControl()->GetSelectedText().Length();
        XRCCTRL(*this, "rbScope", wxRadioBox)->SetSelection(hasSel ? 0 : 1);
	}
	else
        XRCCTRL(*this, "rbScope", wxRadioBox)->SetSelection(1);

    int mode = Manager::Get()->GetConfigManager(_T("app"))->ReadInt(_T("/print_mode"), 1);
    XRCCTRL(*this, "rbColourMode", wxRadioBox)->SetSelection(mode);
    bool print_line_numbers = Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/print_line_numbers"), true);
    XRCCTRL(*this, "chkLineNumbers", wxCheckBox)->SetValue(print_line_numbers);
}

PrintDialog::~PrintDialog()
{
	//dtor
}

PrintScope PrintDialog::GetPrintScope() const
{
    return (PrintScope)XRCCTRL(*this, "rbScope", wxRadioBox)->GetSelection();
}

PrintColourMode PrintDialog::GetPrintColourMode() const
{
    return (PrintColourMode)XRCCTRL(*this, "rbColourMode", wxRadioBox)->GetSelection();
}

bool PrintDialog::GetPrintLineNumbers() const
{
    return XRCCTRL(*this, "chkLineNumbers", wxCheckBox)->GetValue();
}

void PrintDialog::EndModal(int retCode)
{
    if (retCode == wxID_OK)
    {
        int mode = XRCCTRL(*this, "rbColourMode", wxRadioBox)->GetSelection();
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/print_mode"), (int)mode);
        Manager::Get()->GetConfigManager(_T("app"))->Write(_T("/print_line_numbers"), GetPrintLineNumbers());
    }
    return wxScrollingDialog::EndModal(retCode);
}
