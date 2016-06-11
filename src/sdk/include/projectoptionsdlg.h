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

#ifndef PROJECTOPTIONSDLG_H
#define PROJECTOPTIONSDLG_H

#include "settings.h"
#include "scrollingdialog.h"

class wxTreeEvent;
class wxSpinEvent;
class cbProject;
class cbCompilerPlugin;

/*
 * No description
 */
class ProjectOptionsDlg : public wxScrollingDialog
{
    public:
        // class constructor
        ProjectOptionsDlg(wxWindow* parent, cbProject* project);
        // class destructor
        ~ProjectOptionsDlg();

        void EndModal(int retCode);
        void OnFileOptionsClick(wxCommandEvent& event);
        void OnFileToggleMarkClick(wxCommandEvent& event);
        void OnProjectTypeChanged(wxCommandEvent& event);
        void OnBuildTargetChanged(wxCommandEvent& event);
        void OnAddBuildTargetClick(wxCommandEvent& event);
        void OnEditBuildTargetClick(wxCommandEvent& event);
        void OnCopyBuildTargetClick(wxCommandEvent& event);
        void OnRemoveBuildTargetClick(wxCommandEvent& event);
        void OnBrowseOutputFilenameClick(wxCommandEvent& event);
        void OnBrowseDirClick(wxCommandEvent& event);
        void OnBrowseMakeClick(wxCommandEvent& event);
        void OnVirtualTargets(wxCommandEvent& event);
        void OnEditDepsClick(wxCommandEvent& event);
        void OnExportTargetClick(wxCommandEvent& event);
        void OnBuildOrderClick(wxCommandEvent& event);
        void OnProjectBuildOptionsClick(wxCommandEvent& event);
        void OnProjectDepsClick(wxCommandEvent& event);
        void OnTargetBuildOptionsClick(wxCommandEvent& event);
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnOK(wxCommandEvent& event);

        void OnScriptsOverviewSelChanged(wxTreeEvent& event);
        void OnCheckScripts(wxCommandEvent& event);
        void OnAddScript(wxCommandEvent& event);
        void OnRemoveScript(wxCommandEvent& event);
        void OnScriptMoveUp(wxSpinEvent& event);
        void OnScriptMoveDown(wxSpinEvent& event);
    private:
        void BuildScriptsTree();
        void AddPluginPanels();
        void FillScripts();
        void FillBuildTargets();
        void DoTargetChange(bool saveOld = true);
        void DoBeforeTargetChange(bool force = false);
        bool DoCheckScripts(CompileTargetBase* base);
        bool IsScriptValid(ProjectBuildTarget* target, const wxString& script);
        bool ValidateTargetName(const wxString& name);
        void UpdateTargetControls();
        cbProject* m_Project;
        int m_Current_Sel; // current target selection (when selection changes it is the old selection - handy, eh?)
        cbCompilerPlugin* m_pCompiler;
        ConfigurationPanelsArray m_PluginPanels;
        DECLARE_EVENT_TABLE()
};

#endif // PROJECTOPTIONSDLG_H
