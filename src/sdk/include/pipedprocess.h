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

#ifndef PIPEDPROCESS_H
#define PIPEDPROCESS_H

#include "settings.h"
#include <wx/process.h> // inheriting class' header file
#include <wx/txtstrm.h>
#include <wx/timer.h>

/*
 * No description
 */
class DLLIMPORT PipedProcess : public wxProcess
{
	public:
		// class constructor
        PipedProcess(void** pvThis, wxEvtHandler* parent, int id = wxID_ANY, bool pipe = true, const wxString& dir = wxEmptyString);
		// class destructor
		~PipedProcess();
		virtual int Launch(const wxString& cmd, unsigned int pollingInterval = 100);
		virtual void SendString(const wxString& text);
        virtual bool HasInput();
		virtual int GetPid(){ return m_Pid; }
		void ForfeitStreams();
    protected:
         virtual void OnTerminate(int pid, int status);
		virtual void OnTimer(wxTimerEvent& event);
		virtual void OnIdle(wxIdleEvent& event);
        wxEvtHandler* m_Parent;
		int m_Id;
		int m_Pid;
		wxTimer m_timerPollProcess;
	private:
		void** m_pvThis;
		DECLARE_EVENT_TABLE()
};

#endif // PIPEDPROCESS_H
