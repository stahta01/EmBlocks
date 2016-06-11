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

#ifndef FINDDLG_H
#define FINDDLG_H

#include "findreplacebase.h"

class wxNotebookEvent;
class wxCommandEvent;
class wxActivateEvent;

class FindDlg : public FindReplaceBase
{
    public:
        FindDlg(wxWindow* parent, const wxString& initial = wxEmptyString, bool hasSelection = false, bool findInFilesOnly = false, bool findInFilesActive = false);
        ~FindDlg();
        wxString GetFindString() const;
        wxString GetReplaceString() const{ return wxEmptyString; }
        bool IsFindInFiles() const;
        bool GetDeleteOldSearches() const;
        bool GetMatchWord() const;
        bool GetStartWord() const;
        bool GetMatchCase() const;
        bool GetRegEx() const;
        bool GetAutoWrapSearch() const;
        bool GetFindUsesSelectedText() const;
		bool GetStartFile() const;
		bool GetFixEOLs() const;

        int GetDirection() const;
        int GetOrigin() const;
        int GetScope() const;
        bool GetRecursive() const; // for find in search path
        bool GetHidden() const; // for find in search path
        wxString GetSearchPath() const; // for find in search path
        wxString GetSearchMask() const; // for find in search path
        void UpdateUI();

        void OnFindChange(wxNotebookEvent& event);
        void OnRegEx(wxCommandEvent& event);
        void OnBrowsePath(wxCommandEvent& event);
        void OnRadioBox(wxCommandEvent& event);
        void OnActivate(wxActivateEvent& event);

    private:
        bool m_Complete;
        DECLARE_EVENT_TABLE()
};

#endif // FINDDLG_H
