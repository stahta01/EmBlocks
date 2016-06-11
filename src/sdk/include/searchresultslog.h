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

#ifndef SEARCHRESULTSLOG_H
#define SEARCHRESULTSLOG_H

#include <wx/string.h>
#include "loggers.h"

class wxArrayString;
class wxCommandEvent;

class SearchResultsLog : public ListCtrlLogger, public wxEvtHandler
{
	public:
		SearchResultsLog(const wxArrayString& titles, wxArrayInt& widths);
		virtual ~SearchResultsLog();
		void FocusEntry(size_t index);
		void SetBasePath(const wxString base){ m_Base = base; }

		virtual wxWindow* CreateControl(wxWindow* parent);
	protected:
        void OnDoubleClick(wxCommandEvent& event);
        void SyncEditor(int selIndex);

        wxString m_Base;
	private:
        DECLARE_EVENT_TABLE()
};

#endif // SEARCHRESULTSLOG_H
