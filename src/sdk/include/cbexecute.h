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
/*
 * inline long cbSyncExecute(const wxString& command, wxArrayString& output, wxArrayString& error)
 * inline long cbSyncExecute(const wxString& command, wxArrayString& output)
 *
 * Implements behaviour identical to synchronous wxExecute, but uses *safe* yields
 * and is aware of application shutdowns (will stop polling and send SIGTERM to other process).
 *
 * Can be used in place of wxExecute to avoid possible wxYield() reentrancy problems (code completion and tool manager?)
 */

#ifndef CBEXECUTE
#define CBEXECUTE

#include "manager.h"

class cbExecuteProcess : public wxProcess
{
    wxInputStream *stream_stdout;
    wxInputStream *stream_stderr;

    wxArrayString *std_out;
    wxArrayString *std_err;
    bool running;
    int exitCode;

public:
    cbExecuteProcess(wxArrayString* out, wxArrayString* err) : std_out(out), std_err(err), running(true)
    {
        Redirect();
    };

    void FlushPipe()
    {
        wxString line;
        stream_stdout = GetInputStream();
        stream_stderr = GetErrorStream();

        if(stream_stdout && stream_stderr)
        {
            wxTextInputStream t_stream_stdout(*stream_stdout);
            wxTextInputStream t_stream_stderr(*stream_stderr);

            while(! stream_stdout->Eof() )
            {
                line = t_stream_stdout.ReadLine();
                std_out->Add(line);
            }

            if(std_err)
            {
            while(! stream_stderr->Eof() )
            {
                line = t_stream_stderr.ReadLine();
                std_err->Add(line);
            }
            }
        }
    };

    virtual void OnTerminate(int pid, int status)
    {
        FlushPipe();
        exitCode = status;
        running = false;
    }

    bool Running()
    {
        return running;
    };
    bool ExitCode()
    {
        return exitCode;
    };

};




inline long cbSyncExecute(const wxString& command, wxArrayString& output, wxArrayString& error)
{
    cbExecuteProcess process(&output, &error);

    if(wxExecute(command, wxEXEC_ASYNC, &process) == 0)
        return -1;

    while(process.Running())
        {
            if(Manager::isappShuttingDown())
            {
                process.Kill(wxSIGTERM); // will not work under Windows
                return -1;
            }
            Manager::Yield();
        }
    return process.ExitCode();
}

inline long cbSyncExecute(const wxString& command, wxArrayString& output)
{
    cbExecuteProcess process(&output, 0);

    if(wxExecute(command, wxEXEC_ASYNC, &process) == 0)
        return -1;

    while(process.Running())
        {
            if(Manager::isappShuttingDown())
            {
                process.Kill(wxSIGTERM);
                return -1;
            }
            Manager::Yield();
        }
    return process.ExitCode();
}


#endif