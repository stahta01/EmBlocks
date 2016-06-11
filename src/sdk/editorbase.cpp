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

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/filename.h>
    #include <wx/notebook.h>
    #include <wx/menu.h>
    #include "manager.h"
    #include "editorbase.h"
    #include "cbeditor.h"
    #include "editormanager.h"
    #include "pluginmanager.h"
    #include "cbproject.h" // FileTreeData
    #include "projectmanager.h" // ProjectsArray
    #include <wx/wfstream.h>
#endif

#include "cbauibook.h"

#include "cbstyledtextctrl.h"

// needed for initialization of variables
int editorbase_RegisterId(int id)
{
    wxRegisterId(id);
    return id;
}

struct EditorBaseInternalData
{
    EditorBaseInternalData(EditorBase* owner)
        : m_pOwner(owner),
        m_DisplayingPopupMenu(false),
        m_CloseMe(false)
    {}

    EditorBase* m_pOwner;
    bool m_DisplayingPopupMenu;
    bool m_CloseMe;
};

// The following lines reserve 255 consecutive id's
const int EditorMaxSwitchTo = 255;
const int idSwitchFile1 = wxNewId();
const int idSwitchFileMax = editorbase_RegisterId(idSwitchFile1 + EditorMaxSwitchTo -1);

const int idCloseMe = wxNewId();
const int idCloseAll = wxNewId();
const int idCloseAllOthers = wxNewId();
const int idSaveMe = wxNewId();
const int idSaveAll = wxNewId();
const int idSwitchTo = wxNewId();
const int idGoogle = wxNewId();
const int idGoogleCode = wxNewId();
const int idMsdn = wxNewId();

BEGIN_EVENT_TABLE(EditorBase, wxPanel)
    EVT_MENU_RANGE(idSwitchFile1, idSwitchFileMax,EditorBase::OnContextMenuEntry)
    EVT_MENU(idCloseMe, EditorBase::OnContextMenuEntry)
    EVT_MENU(idCloseAll, EditorBase::OnContextMenuEntry)
    EVT_MENU(idCloseAllOthers, EditorBase::OnContextMenuEntry)
    EVT_MENU(idSaveMe, EditorBase::OnContextMenuEntry)
    EVT_MENU(idSaveAll, EditorBase::OnContextMenuEntry)
    EVT_MENU(idGoogle, EditorBase::OnContextMenuEntry)
    EVT_MENU(idGoogleCode, EditorBase::OnContextMenuEntry)
    EVT_MENU(idMsdn, EditorBase::OnContextMenuEntry)
END_EVENT_TABLE()

EditorBase::EditorBase(wxWindow* parent, const wxString& filename, bool staticPage /*= false */)
        : wxPanel(parent, -1),
        m_IsBuiltinEditor(false),
        m_Shortname(_T("")),
        m_Filename(_T("")),
        m_WinTitle(filename),
        m_AltStatusText(_T("")),
        m_StaticPage(staticPage)
{
    m_pData = new EditorBaseInternalData(this);

    Manager::Get()->GetEditorManager()->AddCustomEditor(this);
    InitFilename(filename);
    SetTitle(m_Shortname);

/*
    CodeBlocksEvent event(cbEVT_EDITOR_OPEN);
    event.SetEditor(this);
    event.SetString(m_Filename);
    Manager::Get()->GetPluginManager()->NotifyPlugins(event);
    */
}

EditorBase::~EditorBase()
{
    if (Manager::Get()->GetEditorManager()) // sanity check
        Manager::Get()->GetEditorManager()->RemoveCustomEditor(this);

    if(!Manager::isappShuttingDown())
    {
        CodeBlocksEvent event(cbEVT_EDITOR_CLOSE);
        event.SetEditor(this);
        event.SetString(m_Filename);
        Manager::Get()->GetPluginManager()->NotifyPlugins(event);
    }
    delete m_pData;
}

const wxString& EditorBase::GetTitle()
{
    return m_WinTitle;
}

void EditorBase::SetTitle(const wxString& newTitle)
{
    m_WinTitle = newTitle;
    int mypage = Manager::Get()->GetEditorManager()->FindPageFromEditor(this);
    if (mypage != -1)
        Manager::Get()->GetEditorManager()->GetNotebook()->SetPageText(mypage, newTitle);
}

void EditorBase::InitFilename(const wxString& filename)
{
    if (filename.IsEmpty())
        m_Filename = realpath(CreateUniqueFilename());
    else
        m_Filename = realpath(filename);

    wxFileName fname;
    fname.Assign(m_Filename);
    m_Shortname = fname.GetFullName();

    // set full filename (including path) as tooltip,
    // if possible add the appropriate project also
    NormalizePath(fname, wxEmptyString);
    wxString toolTip = UnixFilename(fname.GetFullPath());
    cbProject* prj = _nullptr;
    ProjectsArray* projects = Manager::Get()->GetProjectManager()->GetProjects();
    for (unsigned int i = 0; i < projects->GetCount(); ++i)
    {
        prj = projects->Item(i);
        if (prj->GetFileByFilename(toolTip, false))
        {
            toolTip += _("\nProject: ") + prj->GetTitle();
            break;
        }
    }
    cbAuiNotebook* nb = Manager::Get()->GetEditorManager()->GetNotebook();
    if (nb)
        nb->SetTabToolTip(this, toolTip);

    //    Manager::Get()->GetLogManager()->DebugLog("ctor: Filename=%s\nShort=%s", m_Filename.c_str(), m_Shortname.c_str());
}

wxString EditorBase::CreateUniqueFilename()
{
    const wxString prefix = _("Untitled");
    const wxString path = wxGetCwd() + wxFILE_SEP_PATH;
    wxString tmp;
    int iter = 0;
    while (true)
    {
        tmp.Clear();
        tmp << path << prefix << wxString::Format(_T("%d"), iter);
        if (!Manager::Get()->GetEditorManager()->GetEditor(tmp) &&
                !wxFileExists(path + tmp))
        {
            return tmp;
        }
        ++iter;
    }
}

void EditorBase::Activate()
{
    Manager::Get()->GetEditorManager()->SetActiveEditor(this);
}

bool EditorBase::Close()
{
    Destroy();
    return true;
}

bool EditorBase::IsBuiltinEditor() const
{
    return m_IsBuiltinEditor;
}

bool EditorBase::ThereAreOthers() const
{
    return (Manager::Get()->GetEditorManager()->GetEditorsCount() > 1);
}

wxMenu* EditorBase::CreateContextSubMenu(int id) // For context menus
{
    wxMenu* menu = 0;

    if(id == idSwitchTo)
    {
        menu = new wxMenu;
        m_SwitchTo.clear();
        for (int i = 0; i < EditorMaxSwitchTo && i < Manager::Get()->GetEditorManager()->GetEditorsCount(); ++i)
        {
            EditorBase* other = Manager::Get()->GetEditorManager()->GetEditor(i);
            if (!other || other == this)
                continue;
            int id = idSwitchFile1+i;
            m_SwitchTo[id] = other;
            menu->Append(id, other->GetShortName());
        }
        if(!menu->GetMenuItemCount())
        {
            delete menu;
            menu = 0;
        }
    }
    return menu;
}

void EditorBase::BasicAddToContextMenu(wxMenu* popup, ModuleType type)
{
    if (type == mtOpenFilesList)
    {
      popup->Append(idCloseMe, _("Close"));
      popup->Append(idCloseAll, _("Close all"));
      popup->Append(idCloseAllOthers, _("Close all others"));
      popup->AppendSeparator();
      popup->Append(idSaveMe, _("Save"));
      popup->Append(idSaveAll, _("Save all"));
      popup->AppendSeparator();
      // enable/disable some items, based on state
      popup->Enable(idSaveMe, GetModified());

      bool hasOthers = ThereAreOthers();
      popup->Enable(idCloseAll, hasOthers);
      popup->Enable(idCloseAllOthers, hasOthers);
    }
    if (type != mtEditorManager) // no editor
    {
        wxMenu* switchto = CreateContextSubMenu(idSwitchTo);
        if(switchto)
            popup->Append(idSwitchTo, _("Switch to"), switchto);
    }
}

void EditorBase::DisplayContextMenu(const wxPoint& position, ModuleType type)
{
    bool noeditor = (type != mtEditorManager);
    // noeditor:
    // True if context menu belongs to open files tree;
    // False if belongs to cbEditor

    // inform the editors we 're just about to create a context menu
    if (!OnBeforeBuildContextMenu(position, type))
        return;

    wxMenu* popup = new wxMenu;

    if (!noeditor && wxGetKeyState(WXK_CONTROL))
    {
        cbStyledTextCtrl* control = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor()->GetControl();
        wxString text = control->GetSelectedText();
        if (text.IsEmpty())
        {
            const int pos = control->GetCurrentPos();
            text = control->GetTextRange(control->WordStartPosition(pos, true), control->WordEndPosition(pos, true));
        }

        if(wxMinimumVersion<2,6,1>::eval)
        {
            popup->Append(idGoogle, _("Search the Internet for \"") + text + _("\""));
            popup->Append(idMsdn, _("Search MSDN for \"") + text + _("\""));
            popup->Append(idGoogleCode, _("Search Google Code for \"") + text + _("\""));
        }
        lastWord = text;

        wxMenu* switchto = CreateContextSubMenu(idSwitchTo);
        if(switchto)
        {
            popup->AppendSeparator();
            popup->Append(idSwitchTo, _("Switch to"), switchto);
        }
    }
    else if (!noeditor && wxGetKeyState(WXK_ALT))
    {

    }
    else
    {
        // Basic functions
        BasicAddToContextMenu(popup, type);

        // Extended functions, part 1 (virtual)
        AddToContextMenu(popup, type, false);

        // ask other editors / plugins if they need to add any entries in this menu...
        FileTreeData* ftd = new FileTreeData(0, FileTreeData::ftdkUndefined);
        ftd->SetFolder(m_Filename);
        Manager::Get()->GetPluginManager()->AskPluginsForModuleMenu(type, popup, ftd);
        delete ftd;

        // Extended functions, part 2 (virtual)
        AddToContextMenu(popup, type, true);
    }
    // inform the editors we 're done creating a context menu (just about to show it)
    OnAfterBuildContextMenu(type);

    // display menu
    wxPoint clientpos;
    if (position==wxDefaultPosition) // "context menu" key
    {
        // obtain the caret point (on the screen) as we assume
        // that the user wants to work with the keyboard
        cbStyledTextCtrl* const control = Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor()->GetControl();
        clientpos = control->PointFromPosition(control->GetCurrentPos());
    }
    else
    {
        clientpos = ScreenToClient(position);
    }

    m_pData->m_DisplayingPopupMenu = true;
    PopupMenu(popup, clientpos);
    delete popup;
    m_pData->m_DisplayingPopupMenu = false;

    // this code *must* be the last code executed by this function
    // because it *will* invalidate 'this'
    if (m_pData->m_CloseMe)
        Manager::Get()->GetEditorManager()->Close(this);
}

void EditorBase::OnContextMenuEntry(wxCommandEvent& event)
{
    // we have a single event handler for all popup menu entries
    // This was ported from cbEditor and used for the basic operations:
    // Switch to, close, save, etc.

    const int id = event.GetId();
    m_pData->m_CloseMe = false;

    if (id == idCloseMe)
    {
        if (m_pData->m_DisplayingPopupMenu)
            m_pData->m_CloseMe = true; // defer delete 'this' until after PopupMenu() call returns
        else
            Manager::Get()->GetEditorManager()->Close(this);
    }
    else if (id == idCloseAll)
    {
        if (m_pData->m_DisplayingPopupMenu)
        {
            Manager::Get()->GetEditorManager()->CloseAllExcept(this);
            m_pData->m_CloseMe = true; // defer delete 'this' until after PopupMenu() call returns
        }
        else
            Manager::Get()->GetEditorManager()->CloseAll();
    }
    else if (id == idCloseAllOthers)
    {
        Manager::Get()->GetEditorManager()->CloseAllExcept(this);
    }
    else if (id == idSaveMe)
    {
        Save();
    }
    else if (id == idSaveAll)
    {
        Manager::Get()->GetEditorManager()->SaveAll();
    }
    else
    if (id >= idSwitchFile1 && id <= idSwitchFileMax)
    {
        // "Switch to..." item
        EditorBase *const ed = m_SwitchTo[id];
        if (ed)
        {
            Manager::Get()->GetEditorManager()->SetActiveEditor(ed);
        }
        m_SwitchTo.clear();
    }
    else if (wxMinimumVersion<2,6,1>::eval)
    {
        if (id == idGoogleCode)
        {
            wxLaunchDefaultBrowser(wxString(_T("http://www.google.com/codesearch?q=")) << URLEncode(lastWord));
        }
        else if (id == idGoogle)
        {
            wxLaunchDefaultBrowser(wxString(_T("http://www.google.com/search?q=")) << URLEncode(lastWord));
        }
        else if (id == idMsdn)
        {
            wxLaunchDefaultBrowser(wxString(_T("http://social.msdn.microsoft.com/Search/en-US/?query=")) << URLEncode(lastWord) << _T("&ac=8"));
        }
    }
    else
    {
        event.Skip();
    }
}

bool EditorBase::IsContextMenuOpened() const
{
    return m_pData->m_DisplayingPopupMenu;
}