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
    #include <wx/stattext.h>
    #include <wx/settings.h>

    #include "cbeditor.h"
#endif
#include "cbstyledtextctrl.h"

#include "confirmreplacedlg.h"

BEGIN_EVENT_TABLE(ConfirmReplaceDlg, wxScrollingDialog)
    EVT_BUTTON(XRCID("btnYes"),       ConfirmReplaceDlg::OnYes)
    EVT_BUTTON(XRCID("btnNo"),        ConfirmReplaceDlg::OnNo)
    EVT_BUTTON(XRCID("btnAllInFile"), ConfirmReplaceDlg::OnAllInFile)
    EVT_BUTTON(XRCID("btnSkipFile"),  ConfirmReplaceDlg::OnSkipFile)
    EVT_BUTTON(XRCID("btnAll"),       ConfirmReplaceDlg::OnAll)
    EVT_BUTTON(XRCID("btnCancel"),    ConfirmReplaceDlg::OnCancel)
END_EVENT_TABLE()

ConfirmReplaceDlg::ConfirmReplaceDlg(wxWindow* parent, bool replaceInFiles, const wxString& label)
{
    if (replaceInFiles)
        wxXmlResource::Get()->LoadObject(this, parent, _T("dlgConfirmReplaceMultiple"),_T("wxScrollingDialog"));
    else
        wxXmlResource::Get()->LoadObject(this, parent, _T("dlgConfirmReplace"),_T("wxScrollingDialog"));
    XRCCTRL(*this, "lblMessage", wxStaticText)->SetLabel(label);
    this->SetEscapeId(XRCID("btnCancel"));
}

ConfirmReplaceDlg::~ConfirmReplaceDlg()
{
}

void ConfirmReplaceDlg::OnYes(wxCommandEvent& WXUNUSED(event))
{
    EndModal(crYes);
}

void ConfirmReplaceDlg::OnNo(wxCommandEvent& WXUNUSED(event))
{
    EndModal(crNo);
}

void ConfirmReplaceDlg::OnAllInFile(wxCommandEvent& WXUNUSED(event))
{
    EndModal(crAllInFile);
}

void ConfirmReplaceDlg::OnSkipFile(wxCommandEvent& WXUNUSED(event))
{
    EndModal(crSkipFile);
}

void ConfirmReplaceDlg::OnAll(wxCommandEvent& WXUNUSED(event))
{
    EndModal(crAll);
}

void ConfirmReplaceDlg::OnCancel(wxCommandEvent& WXUNUSED(event))
{
    EndModal(crCancel);
}

void ConfirmReplaceDlg::CalcPosition(cbStyledTextCtrl* ed)
{
    if(!ed)
        return;

    int w = 0, h = 0;
    GetSize(&w,&h);

    wxPoint pt = ed->PointFromPosition(ed->GetCurrentPos());
    int ed_width = ed->GetSize().x;
    pt.x = (ed_width - w) / 2;
    if(pt.x < 0)
        pt.x = 0;
    pt = ed->ClientToScreen(pt);

    int lineHeight = ed->TextHeight(ed->GetCurrentLine());

    int screenW = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
    int screenH = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
    // sanity check
    if (w > screenW)
        w = screenW;
    if (h > screenH)
        h = screenH;

    // now we 're where we want to be, but check that the whole window is visible...
    // the main goal here is that the caret *should* be visible...

    // for the horizontal axis, easy stuff
    if (pt.x + w > screenW)
        pt.x = screenW - w;

    // for the vertical axis, more work has to be done...

    // Does it fit 4 lines above current line?
    if (h + 4*lineHeight < pt.y)
        pt.y -= h + 4*lineHeight;
    else if (pt.y + lineHeight + (2*h) <= screenH)
    {
        // One line below current line?
        pt.y += lineHeight;
    }
    else
    {
        // Top of the screen
        pt.y = 0;
    }
    // we should be OK now
    SetSize(pt.x, pt.y, w, h);
}
