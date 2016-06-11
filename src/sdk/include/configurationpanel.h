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

#ifndef CONFIGURATIONPANEL_H
#define CONFIGURATIONPANEL_H

#include "globals.h"
#include "settings.h"
#include "scrollingdialog.h"
#include <wx/panel.h>
#include <wx/string.h>


class wxButton;
class wxWindow;

// In styling helpers
DLLIMPORT wxBitmap MakeDiasbledBitmap(const wxBitmap& bmp, wxColor bgColor, double alpha = 0.6);

/** @brief Base class for plugin configuration panels. */
class DLLIMPORT cbConfigurationPanel : public wxPanel
{
    public:
        cbConfigurationPanel() : m_parentDialog(0) { ; }
        virtual ~cbConfigurationPanel(){}

        /// @return the panel's title.
        virtual wxString GetTitle() const = 0;

        /// @return the panel's bitmap on hover
        virtual wxBitmap GetBitmap() { return wxXmlResource::Get()->LoadBitmap(_T("bmp_generic"));}

        /// Called when the user chooses to apply the configuration.
        virtual void OnApply() = 0;
        /// Called when the user chooses to cancel the configuration.
        virtual void OnCancel() = 0;

        virtual void OnInitPanel() {};

        /// Sets the panel's parent dialog
        void SetParentDialog(wxWindow* dialog)
        {
            m_parentDialog = dialog;
        }
        /// Gets the panel's parent dialog
        wxWindow* GetParentDialog()
        {
            return m_parentDialog;
        }
        /** Call global cbMessageBox with m_parentDialog as parent window when
            no parent window specified */
        int cbMessageBox(const wxString& message, const wxString& caption = wxEmptyString, int style = wxOK, wxWindow *parent = NULL, int x = -1, int y = -1)
        {
            if (parent)
                return ::cbMessageBox(message, caption, style, parent, x, y);
            else
                return ::cbMessageBox(message, caption, style, m_parentDialog, x, y);
        }
    private:
        wxWindow* m_parentDialog;
};

/// @brief A simple dialog that wraps a cbConfigurationPanel.
class DLLIMPORT cbConfigurationDialog : public wxScrollingDialog
{
	public:
		cbConfigurationDialog(wxWindow* parent, int id, const wxString& title = wxEmptyString);
		void AttachConfigurationPanel(cbConfigurationPanel* panel);
		~cbConfigurationDialog();

		void EndModal(int retCode);
		void OnInitDialog(wxInitDialogEvent& event);
	protected:
        cbConfigurationPanel* m_pPanel;
        wxButton* m_pOK;
        wxButton* m_pCancel;
	private:

};

#endif // CONFIGURATIONPANEL_H
