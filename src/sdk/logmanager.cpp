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

#ifndef WX_PRECOMP
#include <wx/bitmap.h>
#endif

#ifndef CB_PRECOMP
#include <wx/log.h>
#endif

#include "logmanager.h"
#include "loggers.h"

template<> LogManager* Mgr<LogManager>::instance = 0;
template<> bool  Mgr<LogManager>::isShutdown = false;

LogSlot::LogSlot()
    : log(0),
      icon(0)
{
}

LogSlot::~LogSlot()
{
    if(log != &g_null_log)
        delete log;
    if (icon)
        delete icon;
}

size_t LogSlot::Index() const
{
    return index;
}

void LogSlot::SetLogger(Logger* in)
{
    if(log != &g_null_log)
        delete log;
    log = in;
}

Logger* LogSlot::GetLogger() const
{
    return log;
}

////////////////////////////////////////////////////////////////////////////////

LogManager::LogManager()
{
    for(size_t i = 0; i < max_logs; ++i)
    {
        slot[i].index = i;
        slot[i].log = &g_null_log;
    }

    SetLog(new StdoutLogger, stdout_log);
    SetLog(new StdoutLogger, app_log);
    SetLog(new StdoutLogger, debug_log);
    slot[stdout_log].title = _T("stdout");
    slot[app_log].title = _T("Em::Blocks");
    slot[debug_log].title = _T("Em::Blocks Debug");

    Register(_T("null"),   new Instantiator<NullLogger>);
    Register(_T("stdout"), new Instantiator<StdoutLogger>);
    Register(_T("text"),   new Instantiator<TextCtrlLogger>);
    Register(_T("file"),   new Instantiator<FileLogger, true>);
}

LogManager::~LogManager()
{
    for(inst_map_t::iterator i = instMap.begin(); i != instMap.end(); ++i)
        delete i->second;
}

void LogManager::ClearLogInternal(int i)
{
    if(i == -1)
    {
        for(i=0; i<(int)max_logs; i++)
            if (slot[i].log!=&g_null_log)
                slot[i].log->Clear();
        return;
    }

    if ((i>=0) && (i<(int)max_logs) && (slot[i].log!=&g_null_log))
        slot[i].log->Clear();
};

size_t LogManager::SetLog(Logger* l, int i)
{
    unsigned int index = i;

    if(i <= no_index)
    {
        for(index = debug_log + 1; index < max_logs; ++index)
        {
            if(slot[index].GetLogger() == &g_null_log)
            {
                slot[index].SetLogger(l);
                return index;
            }
        }

        delete l;
        return invalid_log;
    }

    slot[index].SetLogger(l);
    return index;
}

void LogManager::Log(const wxString& msg, int i, Logger::level lv)
{
    // The '\f' FormFeed is used as window clear
    if( msg.Find(_T('\f')) != wxNOT_FOUND )
    {
        ClearLog(i);
        wxArrayString subString = GetArrayFromString(msg, _T('\f'));
        LogInternal(subString.Last(), i, lv);
    }
    else
    {
        LogInternal(msg, i, lv);
    }
}

void LogManager::NotifyUpdate()
{
    for(size_t i = 0; i < max_logs; ++i)
        if(slot[i].log)
            slot[i].log->UpdateSettings();

}

void LogManager::DeleteLog(int i)
{
    SetLog(&g_null_log, i);
}

LogSlot& LogManager::Slot(int i)
{
    return slot[i];
}

size_t LogManager::FindIndex(Logger* l)
{
    for(unsigned int i = invalid_log; i < max_logs; ++i)
    {
        if(slot[i].log == l)
            return i;
    }
    return invalid_log;
}

wxArrayString LogManager::ListAvailable()
{
    wxArrayString as;

    for(inst_map_t::iterator i = instMap.begin(); i != instMap.end(); ++i)
        as.Add(i->first);

    return as;
}

bool LogManager::FilenameRequired(const wxString& name)
{
    inst_map_t::iterator i = instMap.find(name);

    if(i != instMap.end())
        return i->second->RequiresFilename();
    else
        return false;
}

Logger* LogManager::New(const wxString& name)
{
    inst_map_t::iterator i;

    if((i = instMap.find(name)) != instMap.end())
        return i->second->New();
    else
        return new NullLogger;
}

void LogManager::Register(const wxString& name, InstantiatorBase* ins)
{
    instMap[name] = ins;
}

void LogManager::Panic(const wxString& msg, const wxString& component)
{
    wxString title(_T("Panic: "));
    title.Append(component);

    if(!component)
        title.Append(_T("Em::Blocks"));

    wxSafeShowMessage(title, msg);
};
