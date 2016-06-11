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

#include <sdk_precomp.h>

#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/xrc/xmlres.h>
    #include <wx/spinctrl.h>
    #include <wx/textctrl.h>
    #include <wx/wxscintilla.h>
    #include "editorcolourset.h"
#endif

#include "editkeywordsdlg.h"

BEGIN_EVENT_TABLE(EditKeywordsDlg, wxScrollingDialog)
    EVT_SPINCTRL(-1, EditKeywordsDlg::OnSetChange)
END_EVENT_TABLE()

EditKeywordsDlg::EditKeywordsDlg(wxWindow* parent, EditorColourSet* theme, HighlightLanguage lang)
    : m_pTheme(theme),
    m_Lang(lang)
{
    //ctor
	wxXmlResource::Get()->LoadObject(this, parent, _T("dlgEditLangKeywords"),_T("wxScrollingDialog"));

    spnSet = XRCCTRL(*this, "spnSet", wxSpinCtrl);
	txtKeywords = XRCCTRL(*this, "txtKeywords", wxTextCtrl);

    m_LastSet = spnSet->GetValue() - 1;
	txtKeywords->SetValue(m_pTheme->GetKeywords(m_Lang, m_LastSet));
}

EditKeywordsDlg::~EditKeywordsDlg()
{
    //dtor
}

void EditKeywordsDlg::OnSetChange(wxSpinEvent& /*event*/)
{
	m_pTheme->SetKeywords(m_Lang, m_LastSet, txtKeywords->GetValue());
	m_LastSet = spnSet->GetValue() - 1;
	txtKeywords->SetValue(m_pTheme->GetKeywords(m_Lang, m_LastSet));
}
