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

#ifndef CBTOOL_H
#define CBTOOL_H

#include <wx/string.h>

#define CB_TOOLS_SEPARATOR _T("---separator---")

class cbTool
{
  public:
    enum eLaunchOption
    {
        LAUNCH_NEW_CONSOLE_WINDOW,
        LAUNCH_HIDDEN,
        LAUNCH_VISIBLE,
        LAUNCH_VISIBLE_DETACHED
    };

    cbTool() { m_LaunchOption = LAUNCH_NEW_CONSOLE_WINDOW; m_MenuId = -1; }
    // getters
    wxString GetName() const {return m_Name;}
    wxString GetCommand() const {return m_Command;}
    wxString GetParams() const {return m_Params;}
    wxString GetWorkingDir() const {return m_WorkingDir;}
    eLaunchOption GetLaunchOption() const {return m_LaunchOption;}
    int GetMenuId() const {return m_MenuId;}
    // setters
    void SetName(const wxString& Name) {m_Name = Name;}
    void SetCommand(const wxString& Command) {m_Command = Command;}
    void SetParams(const wxString& Params) {m_Params = Params;}
    void SetWorkingDir(const wxString& WorkingDir) {m_WorkingDir = WorkingDir;}
    void SetLaunchOption(eLaunchOption LaunchOption) {m_LaunchOption = LaunchOption;}
    void SetMenuId(int MenuId) {m_MenuId = MenuId;}
  private:
    wxString m_Name;
    wxString m_Command;
    wxString m_Params;
    wxString m_WorkingDir;
    eLaunchOption m_LaunchOption;
    int m_MenuId;
};

#endif // CBTOOL_H
