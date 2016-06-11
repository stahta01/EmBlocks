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
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>
#endif

#include "cbplugin.h"
#include "dlgaboutplugin.h" // class's header file

// class constructor
dlgAboutPlugin::dlgAboutPlugin(wxWindow* parent, const PluginInfo* pi)
{
	wxXmlResource::Get()->LoadObject(this, parent, _T("dlgAboutPlugin"),_T("wxScrollingDialog"));

	XRCCTRL(*this, "lblTitle", wxStaticText)->SetLabel(pi->title);
	XRCCTRL(*this, "txtDescription", wxTextCtrl)->SetValue(pi->description);
	XRCCTRL(*this, "txtThanksTo", wxTextCtrl)->SetValue(pi->thanksTo);
	XRCCTRL(*this, "txtLicense", wxTextCtrl)->SetValue(pi->license);
	XRCCTRL(*this, "lblName", wxStaticText)->SetLabel(pi->name);
	XRCCTRL(*this, "lblVersion", wxStaticText)->SetLabel(pi->version);
	XRCCTRL(*this, "lblAuthor", wxStaticText)->SetLabel(pi->author);
	XRCCTRL(*this, "lblEmail", wxStaticText)->SetLabel(pi->authorEmail);
	XRCCTRL(*this, "lblWebsite", wxStaticText)->SetLabel(pi->authorWebsite);
}

// class destructor
dlgAboutPlugin::~dlgAboutPlugin()
{
	// insert your code here
}
