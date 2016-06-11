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

#include "settings.h"
#include "scrollingdialog.h"
#include <wx/string.h>
#include <wx/artprov.h>

class wxCheckBox;

/**
Dialog that contains a "Don't annoy me" checkbox.

Using this dialog, the user can select not to display this dialog again.
The dialog can be then re-enabled in "Settings->Environment->Dialogs"...
*/
class DLLIMPORT AnnoyingDialog : public wxScrollingDialog
{
    public:
        enum dStyle
        {
            OK,
            YES_NO,
            YES_NO_CANCEL,
            OK_CANCEL,
            ONE_BUTTON,
            TWO_BUTTONS,
            THREE_BUTTONS
        };

        AnnoyingDialog(const wxString& caption, const wxString& message, const wxArtID icon = wxART_INFORMATION,
                       dStyle style = YES_NO, int defaultReturn = wxID_YES, bool separate = true,
                       const wxString& b1 = wxEmptyString, const wxString& b2 = wxEmptyString, const wxString& b3 = wxEmptyString);
        virtual ~AnnoyingDialog(){}
        virtual int ShowModal();
    private:
        void OnButton(wxCommandEvent& event);
        wxCheckBox *cb;
        bool dontAnnoy;
        int defRet;
        DECLARE_EVENT_TABLE()
};
