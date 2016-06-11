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

#ifndef REPLACEDLG_H
#define REPLACEDLG_H

#include "findreplacebase.h"

class wxComboBox;

class ReplaceDlg : public FindReplaceBase
{
	public:
		ReplaceDlg(wxWindow* parent, const wxString& initial = wxEmptyString, bool hasSelection = false,
            bool findInFilesOnly = false, bool replaceInFilesActive = false);
		~ReplaceDlg();
		wxString GetFindString() const;
		wxString GetReplaceString() const;
		bool IsFindInFiles() const;
		bool IsMultiLine() const;
		bool GetDeleteOldSearches() const;
		bool GetMatchWord() const;
		bool GetStartWord() const;
		bool GetMatchCase() const;
        bool GetStartFile() const;
        bool GetFixEOLs() const;

		bool GetRegEx() const;
		bool GetAutoWrapSearch() const;
		bool GetFindUsesSelectedText() const;
		int GetDirection() const;
		int GetOrigin() const;
		int GetScope() const;
		bool GetRecursive() const{ return false; }
		bool GetHidden() const{ return false; }
		wxString GetSearchPath() const{ return wxEmptyString; }
		wxString GetSearchMask() const{ return wxEmptyString; }

		void OnSize(wxSizeEvent& event);
		void OnFindChange(wxNotebookEvent& event);
        void OnMultiChange(wxCommandEvent& event);
        void OnLimitToChange(wxCommandEvent& event);
		void OnRegEx(wxCommandEvent& event);
		void OnActivate(wxActivateEvent& event);

	private:
        bool m_Complete;
		void FillComboWithLastValues(wxComboBox* combo, const wxString& configKey);
		void SaveComboValues(wxComboBox* combo, const wxString& configKey);
		DECLARE_EVENT_TABLE()
};

#endif // REPLACEDLG_H
