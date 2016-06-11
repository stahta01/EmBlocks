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
    #include "compileoptionsbase.h"
    #include "sdk_events.h"
    #include "manager.h"
    #include "macrosmanager.h"
#endif

namespace { static const bool case_sensitive = platform::windows ? false : true; };

CompileOptionsBase::CompileOptionsBase()
    : m_Modified(false)
{
    m_PreBuildRunMode  = DIS_RUN;
    m_PostBuildRunMode = DIS_RUN;
    //ctor
}

CompileOptionsBase::~CompileOptionsBase()
{
    //dtor
}


void CompileOptionsBase::SetLinkerStrOptions(const wxArrayString& linkerOpts)
{
    if (m_LinkerOptionsStr == linkerOpts)
        return;
    m_LinkerOptionsStr = linkerOpts;
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetLinkerStrOptions() const
{
    return m_LinkerOptionsStr;
}

void CompileOptionsBase::SetLinkLibs(const wxArrayString& linkLibs)
{
    if (m_LinkLibsStr == linkLibs)
        return;
    m_LinkLibsStr = linkLibs;
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetLinkLibs() const
{
    return m_LinkLibsStr;
}


void CompileOptionsBase::SetDeviceStrOptions(const wxArrayString& deviceOpts)
{
    if (m_DeviceOptionsStr == deviceOpts)
        return;
    m_DeviceOptionsStr = deviceOpts;
    SetModified(true);
}


const wxArrayString& CompileOptionsBase::GetDeviceStrOptions() const
{
    return m_DeviceOptionsStr;
}


void CompileOptionsBase::SetCompilerStrOptions(const wxArrayString& compilerOpts)
{
    if (m_CompilerOptionsStr == compilerOpts)
        return;
    m_CompilerOptionsStr = compilerOpts;
    SetModified(true);
}


const wxArrayString& CompileOptionsBase::GetCompilerStrOptions() const
{
    return m_CompilerOptionsStr;
}


void CompileOptionsBase::SetAssemblerStrOptions(const wxArrayString& assemblerOpts)
{
    if (m_AssemblerOptionsStr == assemblerOpts)
        return;
    m_AssemblerOptionsStr = assemblerOpts;
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetAssemblerStrOptions() const
{
    return m_AssemblerOptionsStr;
}


void CompileOptionsBase::SetIncludeDirs(const wxArrayString& includeDirs)
{
    if (m_IncludeDirsStr == includeDirs)
        return;

    // make sure we don't have duplicate entries
    // that's why we don't assign the array but rather copy it entry by entry...

    m_IncludeDirsStr.Clear();
    for (size_t i = 0; i < includeDirs.GetCount(); ++i)
    {
        wxString entry = UnixFilename(includeDirs[i]);
        if (m_IncludeDirsStr.Index(entry, case_sensitive) == wxNOT_FOUND)
            m_IncludeDirsStr.Add(entry);
    }
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetIncludeDirs() const
{
    return m_IncludeDirsStr;
}


void CompileOptionsBase::SetLibDirs(const wxArrayString& libDirs)
{
    if (m_LibDirsStr == libDirs)
        return;

    // make sure we don't have duplicate entries
    // that's why we don't assign the array but rather copy it entry by entry...

    m_LibDirsStr.Clear();
    for (size_t i = 0; i < libDirs.GetCount(); ++i)
    {
        wxString entry = UnixFilename(libDirs[i]);
        if (m_LibDirsStr.Index(entry, case_sensitive) == wxNOT_FOUND)
            m_LibDirsStr.Add(entry);
    }
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetLibDirs() const
{
    return m_LibDirsStr;
}

void CompileOptionsBase::SetBuildScripts(const wxArrayString& scripts)
{
    if (m_ScriptsStr == scripts)
        return;

    // make sure we don't have duplicate entries
    // that's why we don't assign the array but rather copy it entry by entry...

    m_ScriptsStr.Clear();
    for (size_t i = 0; i < scripts.GetCount(); ++i)
    {
        wxString entry = UnixFilename(scripts[i]);
        if (m_ScriptsStr.Index(entry, case_sensitive) == wxNOT_FOUND)
            m_ScriptsStr.Add(entry);
    }
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetBuildScripts() const
{
    return m_ScriptsStr;
}

void CompileOptionsBase::SetCommandsBeforeBuild(const wxArrayString& commands)
{
    if (m_CmdsBeforeStr == commands)
        return;
    m_CmdsBeforeStr = commands;
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetCommandsBeforeBuild() const
{
    return m_CmdsBeforeStr;
}

void CompileOptionsBase::SetCommandsAfterBuild(const wxArrayString& commands)
{
    if (m_CmdsAfterStr == commands)
        return;
    m_CmdsAfterStr = commands;
    SetModified(true);
}

const wxArrayString& CompileOptionsBase::GetCommandsAfterBuild() const
{
    return m_CmdsAfterStr;
}

void CompileOptionsBase::SetPreBuildRunSettings(int mode)
{
    if (m_PreBuildRunMode == mode)
        return;
    m_PreBuildRunMode = (PrePostBuildMode)mode;
    SetModified(true);
}

void CompileOptionsBase::SetPostBuildRunSettings(int mode)
{
    if (m_PostBuildRunMode == mode)
        return;
    m_PostBuildRunMode = (PrePostBuildMode)mode;
    SetModified(true);
}

bool CompileOptionsBase::GetModified() const
{
    return m_Modified;
}

void CompileOptionsBase::SetModified(bool modified)
{
    m_Modified = modified;
}

void CompileOptionsBase::AddLinkerStrOption(const wxString& option)
{
    wxString envopt = option;
    if (m_LinkerOptionsStr.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_LinkerOptionsStr.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::AddLinkLib(const wxString& lib)
{
    wxString envopt = lib;
    if (m_LinkLibsStr.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_LinkLibsStr.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::AddCompilerStrOption(const wxString& option)
{
    wxString envopt = option;
    if (m_CompilerOptionsStr.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_CompilerOptionsStr.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::AddDeviceStrOption(const wxString& option)
{
    wxString envopt = option;
    if (m_DeviceOptionsStr.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_DeviceOptionsStr.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::AddAssemblerStrOption(const wxString& option)
{
    wxString envopt = option;
    if (m_AssemblerOptionsStr.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_AssemblerOptionsStr.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::AddIncludeDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    if (m_IncludeDirsStr.Index(entry, case_sensitive) == wxNOT_FOUND)
    {
        m_IncludeDirsStr.Add(entry);
        SetModified(true);
    }
}

void CompileOptionsBase::AddLibDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    if (m_LibDirsStr.Index(entry, case_sensitive) == wxNOT_FOUND)
    {
        m_LibDirsStr.Add(entry);
        SetModified(true);
    }
}

void CompileOptionsBase::AddCommandsBeforeBuild(const wxString& command)
{
    wxString envopt = command;
    m_CmdsBeforeStr.Add(envopt);
    SetModified(true);
}

void CompileOptionsBase::AddCommandsAfterBuild(const wxString& command)
{
    wxString envopt = command;
    m_CmdsAfterStr.Add(envopt);
    SetModified(true);
}

void CompileOptionsBase::AddBuildScript(const wxString& script)
{
    wxString envopt = UnixFilename(script);
    if (m_ScriptsStr.Index(envopt, case_sensitive) == wxNOT_FOUND)
    {
        m_ScriptsStr.Add(envopt);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveLinkerStrOption(const wxString& option)
{
    wxString envopt = option;
    int idx = m_LinkerOptionsStr.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_LinkerOptionsStr.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveLinkLib(const wxString& lib)
{
    wxString envopt = lib;
    int idx = m_LinkLibsStr.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_LinkLibsStr.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveCompilerStrOption(const wxString& option)
{
    wxString envopt = option;
    int idx = m_CompilerOptionsStr.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_CompilerOptionsStr.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveDeviceStrOption(const wxString& option)
{
    wxString envopt = option;
    int idx = m_DeviceOptionsStr.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_DeviceOptionsStr.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveAssemblerStrOption(const wxString& option)
{
    wxString envopt = option;
    int idx = m_AssemblerOptionsStr.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_AssemblerOptionsStr.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveIncludeDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    int idx = m_IncludeDirsStr.Index(entry, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_IncludeDirsStr.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveLibDir(const wxString& option)
{
    wxString entry = UnixFilename(option);
    int idx = m_LibDirsStr.Index(entry, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_LibDirsStr.RemoveAt(idx);
        SetModified(true);
    }
}

void CompileOptionsBase::RemoveCommandsBeforeBuild(const wxString& command)
{
    wxString envopt = command;
    m_CmdsBeforeStr.Remove(envopt);
    SetModified(true);
}

void CompileOptionsBase::RemoveCommandsAfterBuild(const wxString& command)
{
    wxString envopt = command;
    m_CmdsAfterStr.Remove(envopt);
    SetModified(true);
}

void CompileOptionsBase::RemoveBuildScript(const wxString& script)
{
    wxString envopt = UnixFilename(script);
    int idx = m_ScriptsStr.Index(envopt, case_sensitive);
    if (idx != wxNOT_FOUND)
    {
        m_ScriptsStr.RemoveAt(idx);
        SetModified(true);
    }
}

bool CompileOptionsBase::SetVar(const wxString& key, const wxString& value, bool onlyIfExists)
{
    if (onlyIfExists)
    {
        StringHash::iterator it = m_Vars.find(key);
        if (it == m_Vars.end())
            return false;
        it->second = value;
        return true;
    }

    m_Vars[key] = value;
    SetModified(true);
    return true;
}

bool CompileOptionsBase::UnsetVar(const wxString& key)
{
    StringHash::iterator it = m_Vars.find(key);
    if (it != m_Vars.end())
    {
        m_Vars.erase(it);
        SetModified(true);
        return true;
    }
    return false;
}

void CompileOptionsBase::UnsetAllVars()
{
    m_Vars.clear();
}

const wxString& CompileOptionsBase::GetVar(const wxString& key) const
{
    StringHash::const_iterator it = m_Vars.find(key);
    if (it != m_Vars.end())
        return it->second;

    static wxString emptystring = wxEmptyString;
    return emptystring;
}

const StringHash& CompileOptionsBase::GetAllVars() const
{
    return m_Vars;
}
