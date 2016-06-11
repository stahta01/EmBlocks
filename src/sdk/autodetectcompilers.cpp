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
#include "autodetectcompilers.h"

#ifndef CB_PRECOMP
#include "compiler.h"
#include "compilerfactory.h"
#include <wx/button.h>
#include <wx/intl.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/xrc/xmlres.h>
#endif

BEGIN_EVENT_TABLE(AutoDetectCompilers, wxScrollingDialog)
END_EVENT_TABLE()

AutoDetectCompilers::AutoDetectCompilers(wxWindow* parent)
{
    //ctor
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgAutoDetectCompilers"),_T("wxScrollingDialog"));

    wxListCtrl* list = XRCCTRL(*this, "lcCompilers", wxListCtrl);
    if (list)
    {
        list->ClearAll();
        list->InsertColumn(0, _("Compiler"), wxLIST_FORMAT_LEFT, 240);
        list->InsertColumn(1, _("Status"), wxLIST_FORMAT_LEFT, 76);

        for (size_t i = 0; i < CompilerFactory::GetCompilersCount(); ++i)
        {
            Compiler* compiler = CompilerFactory::GetCompiler(i);
            if (!compiler)
                continue;

            list->InsertItem(list->GetItemCount(), compiler->GetName());

            int idx = list->GetItemCount() - 1;
            if (!compiler->GetParentID().IsEmpty()) // not built-in
                list->SetItem(idx, 1, _("User-defined"));
            else
            {
                wxString path = compiler->GetMasterPath(false);
                AutoDetectResult result = compiler->AutoDetectInstallationDir();

                switch(result)
                {
                case adrDetected:
                    if ( path.IsEmpty() || path == compiler->GetMasterPath(false))
                        list->SetItem(idx, 1, _("Detected"));
                    else if (!path.IsEmpty())
                    {
                        list->SetItem(idx, 1, _("User-defined"));
                        compiler->SetMasterPath(path);
                    }
                    break;

                case adrGuessed:
                    break;

                case adrUndefined:
                    list->SetItem(idx, 1, _("Undefined"));
                    break;
                }

            }
        }
    }
}

AutoDetectCompilers::~AutoDetectCompilers()
{
    //dtor
}

