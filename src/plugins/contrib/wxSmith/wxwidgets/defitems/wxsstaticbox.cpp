/*
* This file is part of wxSmith plugin for Em::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 4 $
* $Id: wxsstaticbox.cpp 4 2013-11-02 15:53:52Z gerard $
* $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsstaticbox.cpp $
*/

#include <wx/statbox.h>
#include "wxsstaticbox.h"

namespace
{
    wxsRegisterItem<wxsStaticBox> Reg(_T("StaticBox"),wxsTWidget,_T("Standard"),70);

    WXS_ST_BEGIN(wxsStaticBoxStyles,_T(""))
        WXS_ST_CATEGORY("wxStaticBox")
        WXS_ST_DEFAULTS()
    WXS_ST_END()

    WXS_EV_BEGIN(wxsStaticBoxEvents)
    WXS_EV_END()
}

wxsStaticBox::wxsStaticBox(wxsItemResData* Data):
    wxsWidget(
        Data,
        &Reg.Info,
        wxsStaticBoxEvents,
        wxsStaticBoxStyles),
    Label(_("Label"))
{}

void wxsStaticBox::OnBuildCreatingCode()
{
    switch ( GetLanguage() )
    {
        case wxsCPP:
        {
            AddHeader(_T("<wx/statbox.h>"),GetInfo().ClassName,hfInPCH);
            #if wxCHECK_VERSION(2, 9, 0)
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %N);\n"),Label.wx_str());
            #else
            Codef(_T("%C(%W, %I, %t, %P, %S, %T, %N);\n"),Label.c_str());
            #endif
            BuildSetupWindowCode();
            return;
        }

        default:
        {
            wxsCodeMarks::Unknown(_T("wxsStaticBox::OnBuildCreatingCode"),GetLanguage());
        }
    }
}

wxObject* wxsStaticBox::OnBuildPreview(wxWindow* Parent,long Flags)
{
    wxStaticBox* Preview = new wxStaticBox(Parent,GetId(),Label,Pos(Parent),Size(Parent),Style());
    return SetupWindow(Preview,Flags);
}

void wxsStaticBox::OnEnumWidgetProperties(long Flags)
{
    WXS_SHORT_STRING(wxsStaticBox,Label,_("Label"),_T("label"),_T(""),false)
}