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

#ifndef LOGMGR_H
#define LOGMGR_H

#include "manager.h"
#include "logger.h"
#include <map>

namespace
{
static const unsigned int max_logs = 32;

inline wxString F(const wxChar* msg, ...)
{
    va_list arg_list;
    va_start(arg_list, msg);
    ::temp_string = wxString::FormatV(msg, arg_list);
    va_end(arg_list);

    return ::temp_string;
};

static NullLogger g_null_log;
}


struct LogSlot
{
    Logger* log;
    size_t index;
    friend class LogManager;

    wxBitmap *icon;
    wxString title;

    LogSlot();
    ~LogSlot();

    size_t Index() const;

    void SetLogger(Logger* in);
    Logger* GetLogger() const;
};


class DLLIMPORT LogManager : public Mgr<LogManager>
{
public:
    struct InstantiatorBase
    {
        virtual Logger* New()
        {
            return 0;
        };
        virtual bool RequiresFilename() const
        {
            return false;
        };
        virtual ~InstantiatorBase() {};
    };
    template<typename type, bool requires_filename = false> struct Instantiator : public InstantiatorBase
    {
        virtual Logger* New()
        {
            return new type;
        };
        virtual bool RequiresFilename() const
        {
            return requires_filename;
        };
    };

private:
    typedef std::map<wxString, InstantiatorBase*> inst_map_t;
    inst_map_t instMap;

    LogSlot slot[max_logs];

    LogManager();
    ~LogManager();


    friend class Mgr<LogManager>;
    friend class Manager;

    void ClearLogInternal(int i);
    void LogInternal(const wxString& msg, int i, Logger::level lv)
    {
        // Em::Blocks if the app is shutting down, skip logging
        if (Manager::IsAppShuttingDown())
            return;
        if ((i>=0) && (i<(int)max_logs) && (slot[i].log!=&g_null_log))
            slot[i].log->Append(msg, lv);
    };

public:
    enum { no_index = -1, invalid_log, stdout_log, app_log, debug_log};

    /* ------------------------------------------------------------------------------------------------------
     * Management functions
     * ------------------------------------------------------------------------------------------------------
     * Unless you are writing your own loggers, you will probably not need any of these.
     * SetLog() transfers ownership of the Logger object to the LogManager. Loggers must be heap allocated.
     * On error, SetLog() returns invalid_log
     */
    size_t SetLog(Logger* l, int index = no_index);
    void DeleteLog(int i);
    LogSlot& Slot(int i);
    size_t FindIndex(Logger* l);

    /* ------------------------------------------------------------------------------------------------------
     * Logging functions
     * ------------------------------------------------------------------------------------------------------
     * This section is what most people will be interested in.
     *   - Log(), LogWarning(), and LogError() output "info", "warning", or "error" messages to a log.
     *     Log() is almost certainly the function that you want to use, if you don't know what to pick.
     *     By default, logging is directed to the application log.
     *   - DebugLog() and DebugLogError() direct their output into the debug log.
     *   - LogToStdOut() outputs a message on stdout. Normally you will not want to use this function, it
     *     exists solely for some special cases.
     *   - Panic() signals a condition that does not allow proper continuation of the application or significant
     *     parts of it, but it is slightly less harsh than simply bailing out with an unhandled exception.
     *     Currently, Panic() is simply a wrapper around wxSafeShowMessage(), but it might do something else, too.
     *     When signalling panic, you will usually want to shut down the application as soon as appropriate.
     *     Plugins should call Panic() with the plugin's name as the component argument.
     */

    void Log(const wxString& msg, int i = app_log, Logger::level lv = Logger::info);

    void LogWarning(const wxString& msg, int i = app_log)
    {
        Log(msg, i, Logger::warning);
    };
    void LogError(const wxString& msg, int i = app_log)
    {
        Log(msg, i, Logger::error);
    };

    void Panic(const wxString& msg, const wxString& component = wxEmptyString);

    void DebugLog(const wxString& msg, Logger::level lv = Logger::info)
    {
        Log(msg, debug_log, lv);
    };
    void DebugLogError(const wxString& msg)
    {
        DebugLog(msg, Logger::error);
    };
    void DebugLogWarning(const wxString& msg)
    {
        DebugLog(msg, Logger::warning);
    };

    void LogToStdOut(const wxString& msg, Logger::level lv = Logger::info)
    {
        Log(msg, stdout_log, lv);
    };

    void ClearLog(int i)
    {
        ClearLogInternal(i);
    };

    /* ------------------------------------------------------------------------------------------------------
     * Logger registry and RTTI
     * ------------------------------------------------------------------------------------------------------
     * These functions allow to obtain a list of names for all generic Loggers that are presently available
     * and to create a new Logger by name without knowing the type at compile time.
     *
     *   logptr = LogManager::Get()->New(_T("stdout"));  // does exactly the same as
     *   logptr = new StdoutLogger();
     *
     * You normally do not need to worry about creating Loggers. Only ever consider using these functions if you
     * really have to (want to) change the global behaviour of Em::Blocks for some reason.
     */
    wxArrayString ListAvailable();
    Logger* New(const wxString& name);
    bool FilenameRequired(const wxString& name);
    /*
     * Add a logger to the registry of "available Logger types". Unless you are adding a general Logger which should
     * be accessible by name, you will not need this function. Having said that, you probably NEVER need this function.
     */
    void Register(const wxString& name, InstantiatorBase* ins);

    /* ------------------------------------------------------------------------------------------------------
     *  Unless your name is "main.cpp" by any chance, you don't ever need to call this.
     *  If you use it, and your name is not "main.cpp", then you better come up with a good excuse, if someone asks.
     * ------------------------------------------------------------------------------------------------------
     */
    void NotifyUpdate();
};

#endif
