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

#ifndef VIRTUALBUILDTARGETSDLG_H
#define VIRTUALBUILDTARGETSDLG_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//(*Headers(VirtualBuildTargetsDlg)
#include <wx/checklst.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include "scrollingdialog.h"
//*)

class cbProject;

class VirtualBuildTargetsDlg: public wxScrollingDialog
{
    public:

        VirtualBuildTargetsDlg(wxWindow* parent,wxWindowID id, cbProject* project);
        virtual ~VirtualBuildTargetsDlg();

        //(*Identifiers(VirtualBuildTargetsDlg)
        //*)

    protected:

        //(*Handlers(VirtualBuildTargetsDlg)
        void OnAddClick(wxCommandEvent& event);
        void OnEditClick(wxCommandEvent& event);
        void OnRemoveClick(wxCommandEvent& event);
        void OnAliasesSelect(wxCommandEvent& event);
        void OnTargetsToggled(wxCommandEvent& event);
        //*)

        void OnUpdateUI(wxUpdateUIEvent& event);

        //(*Declarations(VirtualBuildTargetsDlg)
        wxListBox* lstAliases;
        wxButton* btnAdd;
        wxButton* btnRemove;
        wxButton* btnEdit;
        wxCheckListBox* lstTargets;
        //*)

        void SetVirtualTarget(const wxString& targetName);
        void CheckTargets();
        cbProject* m_pProject;

    private:

        DECLARE_EVENT_TABLE()
};

#endif
