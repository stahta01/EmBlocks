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

#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H

#include <wx/event.h>
#include "manager.h"
#include "menuitemsmanager.h"
#include "cbtool.h"

WX_DECLARE_LIST(cbTool, ToolsList);

// forward decls
class wxMenuBar;
class wxMenu;
class wxMenuItem;
class PipedProcess;
class CodeBlocksEvent;

class ToolsManager : public Mgr<ToolsManager>, public wxEvtHandler
{
	public:
		friend class Mgr<ToolsManager>;
		friend class Manager; // give Manager access to our private members
		void CreateMenu(wxMenuBar* menuBar);
		void ReleaseMenu(wxMenuBar* menuBar);
		void AddTool(const cbTool* tool, bool save = true);
		void InsertTool(int position, const cbTool* tool, bool save = true);
		void RemoveToolByIndex(int index);
		cbTool* GetToolByMenuId(int id);
		cbTool* GetToolByIndex(int index);
		int GetToolsCount() const { return m_Tools.GetCount(); }
		void BuildToolsMenu(wxMenu* menu);
		void OnToolClick(wxCommandEvent& event);
		void OnIdle(wxIdleEvent& event);
		void OnToolStdOutput(CodeBlocksEvent& event);
		void OnToolErrOutput(CodeBlocksEvent& event);
		void OnToolTerminated(CodeBlocksEvent& event);
		void OnConfigure(wxCommandEvent& event);
	private:
		ToolsManager();
		~ToolsManager();
		void DoRemoveTool(ToolsList::Node* node);
		int Configure();
		bool Execute(const cbTool* tool);
		void LoadTools();
		void SaveTools();

		ToolsList        m_Tools;
		MenuItemsManager m_ItemsManager;
		wxMenu*          m_Menu;
		PipedProcess*    m_pProcess;
		int              m_Pid;

		DECLARE_EVENT_TABLE()
};

#endif // TOOLSMANAGER_H
