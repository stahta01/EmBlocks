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

#ifndef LOGGER_H
#define LOGGER_H

#include <wx/string.h>
#include "settings.h" // DLLIMPORT

class wxWindow;

namespace
{
    static wxString temp_string(_T('\0'), 250);
    static wxString newline_string(_T("\n"));
}

class DLLIMPORT Logger
{
public:
    /*
    *  The ubiquitous, standard log levels to use are:  info, warning, error, critical
    *
    *  info      - I'm telling you, but you probably won't bother reading anyway.
    *  warning   - You should know about this. I'm telling you now, so you can's sue me later.
    *  error     - Something failed, but the world is not going to end.
    *  critical  - Something failed, and it hit you right in the eye. It really hurts, you have to do something.
    *
    *  Other log levels are for special uses and may have side effects that you don't know about,
    *  and they may not work like expected under the specific conditions they run in... don't use them.
    */
	enum level { caption, info, warning, success, error, critical, failure, pagetitle, spacer, asterisk };
	enum { num_levels = asterisk +1 };

    Logger() {};
    virtual ~Logger() {};


	/* Logger writers:
	*  This is the One Function you must implement. Everything else is optional or bull.
	*  It must be possible to call this function in presence and in absence of GUI without crashing the application.
	*  It is not necessary to provide any actual output at all times, but it must be 100% safe to call this function at all times.
	*  You may not throw from this function, it must return in finite time, and it must not call logging functions (to prevent infinite recursion).
	*  Other than that, you can do anything you want with the log messages that you receive.
	*/
    virtual void Append(const wxString& msg, Logger::level lv = info) = 0;


    virtual void Clear(){};
    virtual void CopyContentsToClipboard(bool /*selectionOnly = false*/) {};

    virtual void UpdateSettings() {};
	virtual wxWindow* CreateControl(wxWindow* /*parent*/) { return 0; };
};



class DLLIMPORT NullLogger : public Logger
{
public:
    virtual void Append(const wxString& /*msg*/, Logger::level /*lv*/){};
};


#endif
