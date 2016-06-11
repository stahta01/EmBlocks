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
#ifdef __WXMAC__
#include <wx/font.h>
#endif //__WXMAC__
#include <wx/image.h>    // wxImage
#include <wx/intl.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>
#include "licenses.h"
#include "configmanager.h"
#endif
#include <wx/bitmap.h>
#include <wx/dcmemory.h>    // wxMemoryDC
#include <wx/statbmp.h>
#include "appglobals.h"
#include "dlgabout.h" // class's header file
#include "configmanager.h"

// class constructor

dlgAbout::dlgAbout(wxWindow* parent)
{
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgAbout"),_T("wxScrollingDialog"));

    const wxString description = _("Welcome to ") + appglobals::AppName + _T(" ") +
                                 appglobals::AppVersion + _T("!\n\n") + appglobals::AppName +
                                 _(" is a full-featured IDE (Integrated Development Environment) "
                                   "aiming the embedded software developer (and the development team). "
                                   "With various compiler and debugger plugins (toolchains) for different kind of "
                                   "micro-controllers.");

    wxString file = ConfigManager::ReadDataPath() + _T("/images/splash.png");


    wxStaticBitmap *bmpControl = XRCCTRL(*this, "lblTitle", wxStaticBitmap);

    wxBitmap bmp = wxXmlResource::Get()->LoadBitmap(_T("bmp_splash"));
    wxMemoryDC dc;
    dc.SelectObject(bmp);

    {  // keep this (kind of) in sync with splashscreen.cpp!
        static const wxString release(wxT(RELEASE));
        static const wxString revision = ConfigManager::GetRevisionString();

        wxFont largeFont(16, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        wxFont smallFont(9,  wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

        wxCoord lf_width, lf_heigth, sf_width, sf_heigth, sm_width, sm_height;

        dc.GetTextExtent(release,  &lf_width, &lf_heigth, 0, 0, &largeFont);
        dc.GetTextExtent(revision, &sf_width, &sf_heigth, 0, 0, &smallFont);

        dc.GetTextExtent(_("SAFE MODE"), &sm_width, &sm_height, 0, 0, &largeFont);

        int x_offset = 350;
        int y_offset = 185;

        lf_width >>= 1;
        sf_width >>= 1;
        int y      = y_offset - ((lf_heigth + sf_heigth + 8) >> 1);

        wxColor txtColor(65,60,117);
        dc.SetTextForeground(txtColor); //*wxBLACK);

        dc.SetFont(largeFont);
        dc.DrawText(release,  x_offset - lf_width, y + (lf_heigth >> 1));
    }

    bmpControl->SetBitmap(bmp);

    XRCCTRL(*this, "lblBuildTimestamp", wxStaticText)->SetLabel(wxString(_("Build: ")) + appglobals::AppBuildTimestamp);
    XRCCTRL(*this, "txtDescription", wxTextCtrl)->SetValue(description);
    XRCCTRL(*this, "txtThanksTo", wxTextCtrl)->SetValue(_(
        "The Code::Blocks team for the original framework (http://www.codeblocks.org/).\n"
        "The wxWidgets project (http://www.wxwidgets.org).\n"
        "wxScintilla (http://sourceforge.net/projects/wxscintilla).\n"
        "TinyXML parser (http://www.grinninglizard.com/tinyxml).\n"
        "Squirrel scripting language (http://www.squirrel-lang.org).\n"
        "The GNU Software Foundation (http://www.gnu.org).\n"
        "Last, but not least, the open-source community."));
    XRCCTRL(*this, "txtLicense", wxTextCtrl)->SetValue(LICENSE_GPL);
    XRCCTRL(*this, "lblName", wxStaticText)->SetLabel(appglobals::AppName);
    XRCCTRL(*this, "lblVersion", wxStaticText)->SetLabel(appglobals::AppActualVersionVerb);
    XRCCTRL(*this, "lblSDK", wxStaticText)->SetLabel(appglobals::AppSDKVersion);
    XRCCTRL(*this, "lblAuthor", wxStaticText)->SetLabel(_("Gerard Zagema"));
    XRCCTRL(*this, "lblEmail", wxStaticText)->SetLabel(appglobals::AppContactEmail);
    XRCCTRL(*this, "lblWebsite", wxStaticText)->SetLabel(appglobals::AppUrl);

#ifdef __WXMAC__
    // Courier 8 point is not readable on Mac OS X, increase font size:
    wxFont font1 = XRCCTRL(*this, "txtThanksTo", wxTextCtrl)->GetFont();
    font1.SetPointSize(10);
    XRCCTRL(*this, "txtThanksTo", wxTextCtrl)->SetFont(font1);
    wxFont font2 = XRCCTRL(*this, "txtLicense", wxTextCtrl)->GetFont();
    font2.SetPointSize(10);
    XRCCTRL(*this, "txtLicense", wxTextCtrl)->SetFont(font2);
#endif
    Fit();
}

// class destructor
dlgAbout::~dlgAbout()
{
    // insert your code here
}
