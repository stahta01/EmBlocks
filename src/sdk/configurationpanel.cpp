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

#include "configurationpanel.h"
#include <wx/intl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/image.h>


wxBitmap MakeDiasbledBitmap(const wxBitmap& bmp, wxColor bgColor, double alpha)
{
    wxImage img = bmp.ConvertToImage();
    img = img.ConvertToGreyscale();
    return wxBitmap(wxBitmap(img).BlendColor(bgColor, alpha));
}


cbConfigurationDialog::cbConfigurationDialog(wxWindow* parent, int id, const wxString& title)
    : wxScrollingDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX),
    m_pPanel(0)
{
    Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&cbConfigurationDialog::OnInitDialog);
}

 void cbConfigurationDialog::OnInitDialog(wxInitDialogEvent& event)
 {
    if(m_pPanel)
        m_pPanel->OnInitPanel();
    wxScrollingDialog::OnInitDialog(event);
 }


void cbConfigurationDialog::AttachConfigurationPanel(cbConfigurationPanel* panel)
{
    assert(panel);

    m_pPanel = panel;
    m_pPanel->Reparent(this);

    wxBoxSizer* bs = new wxBoxSizer(wxVERTICAL);
    bs->Add(m_pPanel, 1, wxGROW | wxRIGHT | wxTOP | wxBOTTOM, 8);

    wxStaticLine* line = new wxStaticLine(this);
    bs->Add(line, 0, wxGROW | wxLEFT | wxRIGHT, 8);

    m_pOK = new wxButton(this, wxID_OK, _("&OK"));
    m_pOK->SetDefault();
    m_pCancel = new wxButton(this, wxID_CANCEL, _("&Cancel"));
    wxStdDialogButtonSizer* but = new wxStdDialogButtonSizer;
    but->AddButton(m_pOK);
    but->AddButton(m_pCancel);
    but->Realize();
    bs->Add(but, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 8);

    SetSizer(bs);

    bs->SetSizeHints(this);
    CentreOnParent();
}

cbConfigurationDialog::~cbConfigurationDialog()
{
}

void cbConfigurationDialog::EndModal(int retCode)
{
    if (retCode == wxID_OK)
        m_pPanel->OnApply();
    else
        m_pPanel->OnCancel();
    wxScrollingDialog::EndModal(retCode);
}
