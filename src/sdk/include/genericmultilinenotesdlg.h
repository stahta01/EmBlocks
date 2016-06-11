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
#ifndef GENERICMULTILINENOTESDLG_H
#define GENERICMULTILINENOTESDLG_H

#include "scrollingdialog.h"
#include <wx/intl.h>


class GenericMultiLineNotesDlg : public wxScrollingDialog
{
    public:
        GenericMultiLineNotesDlg(wxWindow* parent, const wxString& caption = _("Notes"), const wxString& notes = wxEmptyString, bool readOnly = true);
        virtual ~GenericMultiLineNotesDlg();
		GenericMultiLineNotesDlg& operator=(const GenericMultiLineNotesDlg&){ return *this; } // just to satisfy script bindings (never used)

        const wxString& GetNotes(){ return m_Notes; }
        void EndModal(int retCode);
    protected:
        wxString m_Notes;
        bool m_ReadOnly;
    private:
        DECLARE_EVENT_TABLE()
};



#endif // GENERICMULTILINENOTESDLG_H
