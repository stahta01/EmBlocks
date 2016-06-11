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
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "cbexception.h"
#include "configmanager.h"
#include "manager.h"
#include "globals.h"
#endif
#include <wx/statbmp.h>
#include "annoyingdialog.h"

BEGIN_EVENT_TABLE(AnnoyingDialog, wxScrollingDialog)
    EVT_BUTTON(-1, AnnoyingDialog::OnButton)
END_EVENT_TABLE()

AnnoyingDialog::AnnoyingDialog(const wxString& caption, const wxString& message, const wxArtID icon,
                               dStyle style, int defaultReturn, bool /*separate*/,
                               const wxString& b1, const wxString& b2, const wxString& b3)
        : wxScrollingDialog(NULL, -1, caption, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSTAY_ON_TOP),
        cb(0),
        dontAnnoy(false),
        defRet(defaultReturn)
{
	// Em::Blocks needs wxWidgets 2.8
	CompileTimeAssertion<wxMinimumVersion<2,8>::eval>::Assert();

    ConfigManagerContainer::StringSet disabled = Manager::Get()->GetConfigManager(_T("an_dlg"))->ReadSSet(_T("/disabled"));
    if(disabled.find(caption) != disabled.end())
    {
        dontAnnoy = true;
        return;
    }

    wxBoxSizer *outerSizer = new wxBoxSizer( wxVERTICAL );

    wxFlexGridSizer *mainArea = new wxFlexGridSizer(2, 0, 0);
    wxStaticBitmap *bitmap = new wxStaticBitmap(this, -1, wxArtProvider::GetBitmap(icon,  wxART_MESSAGE_BOX), wxDefaultPosition);
    mainArea->Add(bitmap, 0, wxALL, 5);

    wxStaticText *txt = new wxStaticText(this, -1, message, wxDefaultPosition, wxDefaultSize, 0);
    mainArea->Add( txt, 0, wxALIGN_CENTER|wxALL, 5 );

    mainArea->Add( 1, 1, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5 );

    int numButtons = 0;
    int id1 = -1;
    int id2 = -1;
    int id3 = -1;
    wxString bTxt1;
    wxString bTxt2;
    wxString bTxt3;

    if(style == OK || style == ONE_BUTTON)
    {
        numButtons = 1;
        id1 = (style == OK ? wxID_OK : 1);
        bTxt1 = b1.IsEmpty() ? wxString(_("&OK")) : b1;
    }
    else if(style == YES_NO || style == OK_CANCEL || style == TWO_BUTTONS)
    {
        numButtons = 2;
        id1 = (style == YES_NO ? wxID_YES : (style == OK_CANCEL ? wxID_OK     : 1));
        id2 = (style == YES_NO ? wxID_NO  : (style == OK_CANCEL ? wxID_CANCEL : 2));
        bTxt1 = b1.IsEmpty() ? (style == YES_NO ? wxString(_("&Yes")) : wxString(_("&OK")))     : b1;
        bTxt2 = b2.IsEmpty() ? (style == YES_NO ? wxString(_("&No"))  : wxString(_("&Cancel"))) : b2;
    }
    else if(style == YES_NO_CANCEL || style == THREE_BUTTONS)
    {
        numButtons = 3;
        id1 = (style == YES_NO_CANCEL ? wxID_YES    : 1);
        id2 = (style == YES_NO_CANCEL ? wxID_NO     : 2);
        id3 = (style == YES_NO_CANCEL ? wxID_CANCEL : 3);
        bTxt1 = b1.IsEmpty() ? wxString(_("&Yes")) : b1;
        bTxt2 = b2.IsEmpty() ? wxString(_("&No")) : b2;
        bTxt3 = b3.IsEmpty() ? wxString(_("&Cancel")) : b3;
    }
    else
        cbThrow(wxString(_T("Fatal error:\nUndefined style in dialog ")) << caption);

	wxSizer* buttonSizer = 0;
	if (style < ONE_BUTTON) // standard buttons? use wxStdDialogButtonSizer
	{
		wxStdDialogButtonSizer *buttonArea = new wxStdDialogButtonSizer();

		wxButton *but1 = new wxButton(this, id1, bTxt1, wxDefaultPosition, wxDefaultSize, 0);
		but1->SetDefault();
		buttonArea->AddButton(but1);

		if(numButtons > 1)
		{
			wxButton *but2 = new wxButton(this, id2, bTxt2, wxDefaultPosition, wxDefaultSize, 0);
			but2->SetDefault();
			buttonArea->AddButton(but2);
		}
		if(numButtons > 2)
		{
			wxButton *but3 = new wxButton(this, id3, bTxt3, wxDefaultPosition, wxDefaultSize, 0);
			but3->SetDefault();
			buttonArea->AddButton(but3);
		}
		buttonArea->Realize();
		buttonSizer = buttonArea;
	}
	else
	{
		// wxStdDialogButtonSizer accepts only standard IDs for its buttons, so we can't use
		// it with custom buttons
		buttonSizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton *but1 = new wxButton(this, id1, bTxt1, wxDefaultPosition, wxDefaultSize, 0);
		but1->SetDefault();
		buttonSizer->Add(but1, 0, wxRIGHT, 5);

		if(numButtons > 1)
		{
			wxButton *but2 = new wxButton(this, id2, bTxt2, wxDefaultPosition, wxDefaultSize, 0);
			but2->SetDefault();
			buttonSizer->Add(but2, 0, wxRIGHT, 5);
		}
		if(numButtons > 2)
		{
			wxButton *but3 = new wxButton(this, id3, bTxt3, wxDefaultPosition, wxDefaultSize, 0);
			but3->SetDefault();
			buttonSizer->Add(but3, 0, wxRIGHT, 5);
		}
	}

    outerSizer->Add( mainArea, 0, wxALIGN_CENTER|wxALL, 5);
    outerSizer->Add( buttonSizer, 0, wxALIGN_CENTER);

    cb = new wxCheckBox(this, -1, _("Don't annoy me again!"), wxDefaultPosition, wxDefaultSize, 0);
    outerSizer->Add(cb, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    SetSizer( outerSizer );
    outerSizer->SetSizeHints(this);

    Centre();
}

void AnnoyingDialog::OnButton(wxCommandEvent& event)
{
    if(!cb)
        cbThrow(_T("Ow... null pointer."));

    if(cb->IsChecked())
    {
        ConfigManagerContainer::StringSet disabled = Manager::Get()->GetConfigManager(_T("an_dlg"))->ReadSSet(_T("/disabled"));
        disabled.insert(GetTitle());
        Manager::Get()->GetConfigManager(_T("an_dlg"))->Write(_T("/disabled"), disabled);
    }
    EndModal(event.GetId());
}

int AnnoyingDialog::ShowModal()
{
    if(dontAnnoy)
        return defRet;
    PlaceWindow(this);
    return wxScrollingDialog::ShowModal();
};
