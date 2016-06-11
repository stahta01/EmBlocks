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
    #include <wx/xrc/xmlres.h>
   // #include <wx/fs_zip.h>
    #include <wx/menu.h>

    #include "manager.h" // class's header file
    #include "sdk_events.h"
    #include "cbexception.h"
    #include "projectmanager.h"
    #include "editormanager.h"
    #include "logmanager.h"
    #include "pluginmanager.h"
    #include "toolsmanager.h"
    #include "macrosmanager.h"
    #include "configmanager.h"
    #include "scriptingmanager.h"
    #include "templatemanager.h"
    #include "personalitymanager.h"
    #include "uservarmanager.h"
    #include "filemanager.h"
    #include "globals.h"
#endif

#include <wx/app.h>    // wxTheApp
#include <wx/toolbar.h>
#include <wx/fs_mem.h>
#include "../src/main.h"

static Manager* instance = 0;

Manager::Manager() : m_pAppWindow(0)
{
}

Manager::~Manager()
{
    // remove all event sinks
    for (EventSinksMap::iterator mit = m_EventSinks.begin(); mit != m_EventSinks.end(); ++mit)
    {
        mit->second.begin();
        while (mit->second.size())
        {
            delete (*(mit->second.begin()));
            mit->second.erase(mit->second.begin());
        }
    }

    for (DockEventSinksMap::iterator mit = m_DockEventSinks.begin(); mit != m_DockEventSinks.end(); ++mit)
    {
        mit->second.begin();
        while (mit->second.size())
        {
            delete (*(mit->second.begin()));
            mit->second.erase(mit->second.begin());
        }
    }

    for (LayoutEventSinksMap::iterator mit = m_LayoutEventSinks.begin(); mit != m_LayoutEventSinks.end(); ++mit)
    {
        mit->second.begin();
        while (mit->second.size())
        {
            delete (*(mit->second.begin()));
            mit->second.erase(mit->second.begin());
        }
    }

    for (LogEventSinksMap::iterator mit = m_LogEventSinks.begin(); mit != m_LogEventSinks.end(); ++mit)
    {
        mit->second.begin();
        while (mit->second.size())
        {
            delete (*(mit->second.begin()));
            mit->second.erase(mit->second.begin());
        }
    }


//    Shutdown();
    CfgMgrBldr::Free(); // only terminate config at the very last moment
//    FileManager::Free();
}


Manager* Manager::Get(wxFrame *appWindow)
{
    if (appWindow)
    {
        if (Get()->m_pAppWindow)
            cbThrow(_T("Illegal argument to Manager::Get()"));
        else
        {
            Get()->m_pAppWindow = appWindow;
            if (!LoadResource(_T("manager_resources.zip")))
                NotifyMissingFile(_T("manager_resources.zip"));
            else
                Get()->GetLogManager()->Log(_("Manager initialized"));
        }
    }
    return Get();
}

Manager* Manager::Get()
{
    if (!instance)
        instance = new Manager;
    return instance;
}

void Manager::Free()
{
    delete instance;
    instance = 0;
}

void Manager::SetBatchBuild(bool is_batch)
{
    isBatch = is_batch;
}

void Manager::BlockYields(bool block)
{
    blockYields = block;
}

void Manager::ProcessPendingEvents()
{
    if (!blockYields && !appShuttingDown)
        wxTheApp->ProcessPendingEvents();
}

void Manager::Yield()
{
    if (!blockYields && !appShuttingDown)
        wxTheApp->Yield(true);
}

void Manager::Shutdown()
{
    appShuttingDown = true;

    ToolsManager::Free();
    TemplateManager::Free();
    PluginManager::Free();
    ScriptingManager::Free();
    ProjectManager::Free();
    EditorManager::Free();
    PersonalityManager::Free();
    MacrosManager::Free();
    UserVariableManager::Free();
    LogManager::Free();
}

bool Manager::ProcessEvent(CodeBlocksEvent& event)
{
    if (IsAppShuttingDown())
        return false;

    EventSinksMap::iterator mit = m_EventSinks.find(event.GetEventType());
    if (mit != m_EventSinks.end())
    {
        for (EventSinksArray::iterator it = mit->second.begin(); it != mit->second.end(); ++it)
            (*it)->Call(event);
    }
    return true;
}

bool Manager::ProcessEvent(CodeBlocksDockEvent& event)
{
    if (IsAppShuttingDown())
        return false;

    DockEventSinksMap::iterator mit = m_DockEventSinks.find(event.GetEventType());
    if (mit != m_DockEventSinks.end())
    {
        for (DockEventSinksArray::iterator it = mit->second.begin(); it != mit->second.end(); ++it)
            (*it)->Call(event);
    }
    return true;
}

bool Manager::ProcessEvent(CodeBlocksLayoutEvent& event)
{
    if (IsAppShuttingDown())
        return false;

    LayoutEventSinksMap::iterator mit = m_LayoutEventSinks.find(event.GetEventType());
    if (mit != m_LayoutEventSinks.end())
    {
        for (LayoutEventSinksArray::iterator it = mit->second.begin(); it != mit->second.end(); ++it)
            (*it)->Call(event);
    }
    return true;
}

bool Manager::ProcessEvent(CodeBlocksLogEvent& event)
{
    if (IsAppShuttingDown())
        return false;

    LogEventSinksMap::iterator mit = m_LogEventSinks.find(event.GetEventType());
    if (mit != m_LogEventSinks.end())
    {
        for (LogEventSinksArray::iterator it = mit->second.begin(); it != mit->second.end(); ++it)
            (*it)->Call(event);
    }
    return true;
}


bool Manager::IsAppShuttingDown()
{
    return appShuttingDown;
}


void Manager::Initxrc(bool force)
{
    static bool xrcok = false;
    if (!xrcok || force)
    {
        //    wxFileSystem::AddHandler(new wxZipFSHandler); // The application is responsible to have the ZIP handler installed
        wxXmlResource::Get()->InitAllHandlers();
        xrcok=true;
    }
}

void Manager::Loadxrc(wxString relpath)
{
    LoadResource(relpath);
}

wxMenuBar *Manager::LoadMenuBar(wxString resid,bool createonfailure)
{
    wxMenuBar *m = wxXmlResource::Get()->LoadMenuBar(resid);
    if (!m && createonfailure) m = new wxMenuBar();
    return m;
}

wxMenu *Manager::LoadMenu(wxString menu_id,bool createonfailure)
{
    wxMenu *m = wxXmlResource::Get()->LoadMenu(menu_id);
    if (!m && createonfailure) m = new wxMenu(_T(""));
    return m;
}
void Manager::AuiToolBar(wxAuiToolBar* toolBar, wxString resid)
{
   if (!toolBar)
        return;

    wxXmlResource::Get()->LoadObject(toolBar,NULL,resid,_T("wxToolBarAddOn"));
}

wxFrame* Manager::GetAppFrame() const
{
    return m_pAppWindow;
}

const wxBitmap& Manager::GetMenuBitmap(int menuId) const
{
    if(m_pAppWindow)
    {
        wxMenuBar *pBar = m_pAppWindow->GetMenuBar();

        if(pBar)
        {
            wxMenuItem* pMenu = pBar->FindItem(menuId);
            if(pMenu)
                return pMenu->GetBitmap();
        }
    }
    return wxNullBitmap;
}

wxWindow* Manager::GetAppWindow() const
{
    return (wxWindow*)m_pAppWindow;
}

ProjectManager* Manager::GetProjectManager() const
{
    return ProjectManager::Get();
}

EditorManager* Manager::GetEditorManager() const
{
    return EditorManager::Get();
}

LogManager* Manager::GetLogManager() const
{
    return LogManager::Get();
}

PluginManager* Manager::GetPluginManager() const
{
    return PluginManager::Get();
}

ToolsManager* Manager::GetToolsManager() const
{
    return ToolsManager::Get();
}

MacrosManager* Manager::GetMacrosManager() const
{
    return MacrosManager::Get();
}

PersonalityManager* Manager::GetPersonalityManager() const
{
    return PersonalityManager::Get();
}

UserVariableManager* Manager::GetUserVariableManager() const
{
    return UserVariableManager::Get();
}

ScriptingManager* Manager::GetScriptingManager() const
{
    return ScriptingManager::Get();
}

ConfigManager* Manager::GetConfigManager(const wxString& name_space) const
{
    return CfgMgrBldr::GetConfigManager(name_space);
}

FileManager* Manager::GetFileManager() const
{
    return FileManager::Get();
}

wxColor Manager::GetLayoutColor(int id) const
{
    wxColor ret;
    wxAuiDockArt* pDart = GetDockArtProvider();
    if(pDart)
        ret = pDart->GetColor(id);
    return ret;
}

wxAuiDockArt* Manager::GetDockArtProvider() const
{
    return ((MainFrame*)m_pAppWindow)->GetLayoutManager()->GetArtProvider();
}

bool Manager::LoadResource(const wxString& file)
{
    wxString resourceFile = ConfigManager::LocateDataFile(file, sdDataGlobal | sdDataUser);
    wxString memoryFile = _T("memory:") + file;

    if (wxFile::Access(resourceFile, wxFile::read) == false)
        return false;

    // The code below forces a reload of the resource
    // Currently unused...

//    {
//        wxMemoryFSHandler::RemoveFile(file);
//    }
//#if wxABI_VERSION > 20601
//    // unload old resources with the same name
//    wxXmlResource::Get()->Unload(memoryFile);
//#endif

    wxFile f(resourceFile, wxFile::read);
    char *buf = 0;

    try
    {
        size_t len = f.Length();
        buf = new char[len];
        f.Read(buf, len);
        {
            wxMemoryFSHandler::AddFile(file, buf, len);
        }
        wxXmlResource::Get()->Load(memoryFile);
        delete[] buf;
        return true;
    }
    catch (...)
    {
        delete[] buf;
        return false;
    }
}

wxCmdLineParser* Manager::GetCmdLineParser()
{
    return &m_CmdLineParser;
}

void Manager::RegisterEventSink(wxEventType eventType, IEventFunctorBase<CodeBlocksEvent>* functor)
{
    m_EventSinks[eventType].push_back(functor);
}

void Manager::RegisterEventSink(wxEventType eventType, IEventFunctorBase<CodeBlocksDockEvent>* functor)
{
    m_DockEventSinks[eventType].push_back(functor);
}

void Manager::RegisterEventSink(wxEventType eventType, IEventFunctorBase<CodeBlocksLayoutEvent>* functor)
{
    m_LayoutEventSinks[eventType].push_back(functor);
}

void Manager::RegisterEventSink(wxEventType eventType, IEventFunctorBase<CodeBlocksLogEvent>* functor)
{
    m_LogEventSinks[eventType].push_back(functor);
}

void Manager::RemoveAllEventSinksFor(void* owner)
{
    for (EventSinksMap::iterator mit = m_EventSinks.begin(); mit != m_EventSinks.end(); ++mit)
    {
        EventSinksArray::iterator it = mit->second.begin();
        bool endIsInvalid = false;
        while (!endIsInvalid && it != mit->second.end())
        {
            if ((*it) && (*it)->GetThis() == owner)
            {
                EventSinksArray::iterator it2 = it++;
                endIsInvalid = it == mit->second.end();
                delete (*it2);
                mit->second.erase(it2);
            }
            else
                ++it;
        }
    }

    for (DockEventSinksMap::iterator mit = m_DockEventSinks.begin(); mit != m_DockEventSinks.end(); ++mit)
    {
        DockEventSinksArray::iterator it = mit->second.begin();
        bool endIsInvalid = false;
        while (!endIsInvalid && it != mit->second.end())
        {
            if ((*it) && (*it)->GetThis() == owner)
            {
                DockEventSinksArray::iterator it2 = it++;
                endIsInvalid = it == mit->second.end();
                delete (*it2);
                mit->second.erase(it2);
            }
            else
                ++it;
        }
    }

    for (LayoutEventSinksMap::iterator mit = m_LayoutEventSinks.begin(); mit != m_LayoutEventSinks.end(); ++mit)
    {
        LayoutEventSinksArray::iterator it = mit->second.begin();
        bool endIsInvalid = false;
        while (!endIsInvalid && it != mit->second.end())
        {
            if ((*it) && (*it)->GetThis() == owner)
            {
                LayoutEventSinksArray::iterator it2 = it++;
                endIsInvalid = it == mit->second.end();
                delete (*it2);
                mit->second.erase(it2);
            }
            else
                ++it;
        }
    }

    for (LogEventSinksMap::iterator mit = m_LogEventSinks.begin(); mit != m_LogEventSinks.end(); ++mit)
    {
        LogEventSinksArray::iterator it = mit->second.begin();
        bool endIsInvalid = false;
        while (!endIsInvalid && it != mit->second.end())
        {
            if ((*it) && (*it)->GetThis() == owner)
            {
                LogEventSinksArray::iterator it2 = it++;
                endIsInvalid = it == mit->second.end();
                delete (*it2);
                mit->second.erase(it2);
            }
            else
                ++it;
        }
    }
}

bool Manager::appShuttingDown = false;
bool Manager::blockYields = false;
bool Manager::isBatch = false;
wxCmdLineParser Manager::m_CmdLineParser;