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

    #include "compiletargetbase.h"
    #include "compilerfactory.h"
    #include "globals.h"
    #include "logmanager.h" // Manager::Get()->GetLogManager()->DebugLog(F())
#endif

#include "filefilters.h"

CompileTargetBase::CompileTargetBase()
    : m_TargetType(ttExecutable)
{
    //ctor
    for (int i = 0; i < static_cast<int>(ortLast); ++i)
        m_OptionsRelation[i] = orAppendToParentOptions;

    // The device settings can only be 0 or 1, so set it at a valid value
    m_OptionsRelation[ortDeviceOptions] = orUseParentOptionsOnly;


    // default "make" commands
    m_MakeCommands[mcBuild]             = _T("$make -f $makefile $target");
    m_MakeCommands[mcCompileFile]       = _T("$make -f $makefile $file");
    m_MakeCommands[mcClean]             = _T("$make -f $makefile clean$target");
    m_MakeCommands[mcDistClean]         = _T("$make -f $makefile distclean$target");
    m_MakeCommands[mcAskRebuildNeeded]  = _T("$make -q -f $makefile $target");
    m_MakeCommands[mcSilentBuild]       = _T("$make -s -f $makefile $target");
    m_MakeCommandsModified = false;
}

CompileTargetBase::~CompileTargetBase()
{
    //dtor
}

const wxString& CompileTargetBase::GetFilename() const
{
    return m_Filename;
}

const wxString& CompileTargetBase::GetTitle() const
{
    return m_Title;
}

void CompileTargetBase::SetTitle(const wxString& title)
{
    if (m_Title == title)
        return;
    m_Title = title;
    SetModified(true);
}

void CompileTargetBase::SetOutputFilename(const wxString& filename)
{
    if (filename.IsEmpty())
    {
        m_OutputFilename = SuggestOutputFilename();
        SetModified(true);
        return;
    }
    else if (m_OutputFilename == filename)
        return;
    m_OutputFilename = UnixFilename(filename);
    GenerateTargetFilename(m_OutputFilename);
    SetModified(true);
}

/*
void CompileTargetBase::SetWorkingDir(const wxString& dirname)
{
    if (m_WorkingDir == dirname)
        return;
    m_WorkingDir = UnixFilename(dirname);
    SetModified(true);
}
*/

void CompileTargetBase::SetObjectOutput(const wxString& dirname)
{
    if (m_ObjectOutput == dirname)
        return;
    m_ObjectOutput = UnixFilename(dirname);
    SetModified(true);
}

void CompileTargetBase::SetDepsOutput(const wxString& dirname)
{
    if (m_DepsOutput == dirname)
        return;
    m_DepsOutput = UnixFilename(dirname);
    SetModified(true);
}

OptionsRelation CompileTargetBase::GetOptionRelation(OptionsRelationType type) const
{
    return m_OptionsRelation[type];
}

void CompileTargetBase::SetOptionRelation(OptionsRelationType type, OptionsRelation rel)
{
    if (m_OptionsRelation[type] == rel)
        return;
    m_OptionsRelation[type] = rel;
    SetModified(true);
}

wxString CompileTargetBase::GetOutputFilename()
{
    if (m_OutputFilename.IsEmpty())
        m_OutputFilename = SuggestOutputFilename();
    return m_OutputFilename;
}

wxString CompileTargetBase::SuggestOutputFilename()
{
    wxString suggestion;
    switch (m_TargetType)
    {
        case ttExecutable:  suggestion = GetExecutableFilename(); break;
        case ttLibrary:     suggestion = GetLibraryFilename(); break;
        default:
            suggestion.Clear();
            break;
    }
    wxFileName fname(suggestion);
    return UnixFilename(fname.GetFullName());
}

/*
wxString CompileTargetBase::GetWorkingDir()
{
    if (m_TargetType != ttExecutable && m_TargetType != ttLibrary)
        return wxEmptyString;
    wxString out;
    if (m_WorkingDir.IsEmpty())
    {
        out = GetOutputFilename();
        return wxFileName(out).GetPath(wxPATH_GET_VOLUME);
    }
    return m_WorkingDir;
}
*/
wxString CompileTargetBase::GetObjectOutput() const
{
    wxString out;
    if (m_ObjectOutput.IsEmpty())
    {
        out = GetBasePath();
        if (out.IsEmpty() || out.Matches(_T(".")))
             return _T(".objs");
        else
            return out + wxFileName::GetPathSeparator() + _T(".objs");
    }
    return m_ObjectOutput;
}

wxString CompileTargetBase::GetDepsOutput() const
{
    wxString out;
    if (m_DepsOutput.IsEmpty())
    {
        out = GetBasePath();
        if (out.IsEmpty() || out.Matches(_T(".")))
             return _T(".deps");
        else
            return out + wxFileName::GetPathSeparator() + _T(".deps");
    }
    return m_DepsOutput;
}

void CompileTargetBase::GenerateTargetFilename(wxString& filename) const
{
    wxFileName fname(filename);
    // path with volume and separator
   // filename << fname.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    // prefix + name + extension

    // Check if there is already an extension by the user (override)
    if(!fname.GetExt().IsEmpty())
        return;

    Compiler* compiler = CompilerFactory::GetCompiler(GetCompilerID());

    switch (m_TargetType)
    {
        case ttExecutable:
            fname.SetExt( compiler ? compiler->GetSwitches().execExtension : FileFilters::EXECUTABLE_DOT_EXT);
            break;

        case ttLibrary:
            fname.SetExt( compiler ? compiler->GetSwitches().libExtension : FileFilters::LIBRARY_DOT_EXT);
            break;

        default:
            //filename.Clear();
            break;
    }

#ifdef command_line_generation
    Manager::Get()->GetLogManager()->DebugLog(F(_T("CompileTargetBase::GenerateTargetFilename got %s and returns: '%s'"), filename.wx_str(), fname.GetFullPath().wx_str() ));
#endif

    filename.Clear();
    filename << fname.GetFullPath();
}

wxString CompileTargetBase::GetExecutableFilename() const
{
    wxString out = m_Filename;
    GenerateTargetFilename(out);
    return out;
}


wxString CompileTargetBase::GetLibraryFilename()
{
    if (m_Filename.IsEmpty())
        m_Filename = m_OutputFilename;

    wxString out = m_Filename;
    GenerateTargetFilename(out);
#ifdef command_line_generation
    Manager::Get()->GetLogManager()->DebugLog(F(_T("CompileTargetBase::GetDynamicLibFilename [0] returns: '%s'"), out.wx_str()));
#endif
    return out;
}

wxString CompileTargetBase::GetBasePath() const
{
    if (m_Filename.IsEmpty())
        return _T(".");

    wxFileName basePath(m_Filename);
    wxString base = basePath.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    return !base.IsEmpty() ? base : _T(".");
}

void CompileTargetBase::SetTargetType(TargetType pt)
{
    if (m_TargetType == pt)
        return;

    m_TargetType = pt;
    m_OutputFilename = SuggestOutputFilename();
    SetModified(true);
}

TargetType CompileTargetBase::GetTargetType() const
{
    return m_TargetType;
}


void CompileTargetBase::SetCompilerID(const wxString& id)
{
    if (id == m_CompilerId)
        return;

    m_CompilerId = id;
    SetModified(true);
}

void CompileTargetBase::SetMakeCommandFor(MakeCommand cmd, const wxString& make)
{
    if (m_MakeCommands[cmd] == make)
        return;

    m_MakeCommands[cmd] = make;
    m_MakeCommandsModified = true;
    SetModified(true);
}
