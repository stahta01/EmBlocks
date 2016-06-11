/*
 * This file is part of the code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
*/
/*
    Copyright (C) Em::Blocks 2011-2013

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


    @version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:

 */

#ifndef SCRIPTCONSOLE_H
#define SCRIPTCONSOLE_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//(*Headers(ScriptConsole)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/bmpbuttn.h>
#include <wx/combobox.h>
//*)

class ScriptConsole: public wxPanel
{
	public:

		ScriptConsole(wxWindow* parent,wxWindowID id = -1);
		virtual ~ScriptConsole();

        void Log(const wxString& msg);

		//(*Identifiers(ScriptConsole)
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT1;
		static const long ID_COMBOBOX1;
		static const long ID_BITMAPBUTTON1;
		static const long ID_BITMAPBUTTON2;
		static const long ID_BITMAPBUTTON3;
		static const long ID_PANEL1;
		//*)

	protected:
		//(*Handlers(ScriptConsole)
		void OnbtnExecuteClick(wxCommandEvent& event);
		void OnbtnLoadClick(wxCommandEvent& event);
		void OnbtnClearClick(wxCommandEvent& event);
		//*)

		//(*Declarations(ScriptConsole)
		wxStaticText* lblCommand;
		wxBitmapButton* btnClear;
		wxComboBox* txtCommand;
		wxBitmapButton* btnExecute;
		wxPanel* Panel1;
		wxBitmapButton* btnLoad;
		wxTextCtrl* txtConsole;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
