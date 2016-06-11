/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef TODOSETTINGSDLG_H
#define TODOSETTINGSDLG_H

#include <wx/intl.h>
#include "configurationpanel.h"
#include <settings.h>

class ToDoSettingsDlg : public cbConfigurationPanel
{
	public:
		ToDoSettingsDlg(wxWindow* parent);
		~ToDoSettingsDlg();

        virtual wxString GetTitle() const { return _("Todo list"); }
        virtual wxBitmap GetBitmap() { return wxXmlResource::Get()->LoadBitmap(_T("bmp_todo"));}
        virtual void OnApply();
        virtual void OnCancel(){}
};

#endif // TODOSETTINGSDLG_H
