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

#ifndef NEWFROMTEMPLATEDLG_H
#define NEWFROMTEMPLATEDLG_H

#include "scrollingdialog.h"
#include "globals.h"
#include "pluginmanager.h"
#include "cbplugin.h"

class wxChoice;
class wxListCtrl;
class wxListEvent;
class ProjectTemplateLoader;

class NewFromTemplateDlg : public wxScrollingDialog
{
	public:
		NewFromTemplateDlg(TemplateOutputType initial, const wxArrayString& user_templates);
		virtual ~NewFromTemplateDlg();

		ProjectTemplateLoader* GetTemplate(){ return m_Template; }
		cbWizardPlugin* GetWizard(){ return m_pWizard; }
		int GetWizardIndex() const { return m_WizardIndex; }
		cbWizardPlugin* GetSelectedTemplate();
		bool SelectedUserTemplate() const;
		wxString GetSelectedUserTemplate() const;

		void EndModal(int retCode);
	protected:
		void FillTemplate(ProjectTemplateLoader* pt);
		void BuildCategories();
		void BuildCategoriesFor(TemplateOutputType otype, wxChoice* cat);
		void BuildList();
		void BuildListFor(TemplateOutputType otype, wxListCtrl* list, const wxChoice* cat);
		void ClearList();
		void ClearListFor(wxListCtrl* list);
		void OnListRightClick(wxListEvent& event);
		void OnListActivate(wxListEvent& event);
		void OnCategoryChanged(wxCommandEvent& event);
		void OnEditScript(wxCommandEvent& event);
		void OnDiscardScript(wxCommandEvent& event);
		void OnEditGlobalScript(wxCommandEvent& event);
		void OnViewChange(wxCommandEvent& event);
		void OnHelp(wxCommandEvent& event);
		void OnUpdateUI(wxUpdateUIEvent& event);

		void ChangeView();
		void EditScript(const wxString& filename);
		wxListCtrl* GetVisibleListCtrl();
		wxChoice* GetVisibleCategory();
		TemplateOutputType GetVisibleOutputType() const;
	private:
		ProjectTemplateLoader* m_Template;
		cbWizardPlugin* m_pWizard;
		int m_WizardIndex;
		PluginsArray m_Wizards;
		DECLARE_EVENT_TABLE()
};

#endif // NEWFROMTEMPLATEDLG_H
