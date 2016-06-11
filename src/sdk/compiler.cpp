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
#include "cbexception.h"
#include "compiler.h"
#include "manager.h"
#include "logmanager.h"
#include "configmanager.h"
#include "macrosmanager.h"
#include "globals.h"
#include "compilerfactory.h"

#include <wx/intl.h>
#include <wx/regex.h>
#endif

#include <wx/filefn.h>
#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(RegExArray);

// static
wxArrayString Compiler::m_CompilerIDs; // map to guarantee unique IDs

// common regex that can be used by the different compiler for matching compiler output
// it can be used in the patterns for warnings, errors, ...
// NOTE : it is an approximation (for example the ':' can appear anywhere and several times)
const wxString Compiler::FilePathWithSpaces = _T("[][{}() \t#!%$~[:alnum:]&_:+/\\.-]+");

// version of compiler settings
// when this is different from what is saved in the config, a message appears
// to the user saying that default settings have changed and asks him if he wants to
// use his own settings or the new defaults
const wxString CompilerSettingsVersion = _T("0.0.2");

CompilerSwitches::CompilerSwitches()
{
    // defaults
    includeDirs             = _T("");
    libDirs                 = _T("");
    linkLibs                = _T("");
    defines                 = _T("");
    lnkdef                  = _T("");
    objectExtension         = _T("");
    needDependencies        = true;
    forceFwdSlashes         = false;
    forceCompilerUseQuotes  = false;
    forceLinkerUseQuotes    = false;
    logging                 = clogSimple;
    libPrefix               = _T("");
    libExtension            = _T("");
    mapExtension            = _T("");
    hexExtension            = _T("");
    execExtension           = _T("");
    linkerNeedsLibPrefix    = false;
    linkerNeedsLibExtension = false;
    supportsPCH             = true;
    PCHExtension            = _T("");
    UseFlatObjects          = false;
    UseFullSourcePaths      = false;
    Use83Paths              = false;
    caseSensitiveExt        = false;
    defines                 = _T("");
    macros                  = _T("");
}

wxString Compiler::CommandTypeDescriptions[ctCount] =
{
    // These are the strings that describe each CommandType enumerator...
    // No need to say that it must have the same order as the enumerators!
    _("Compile single file to object file"),
    _("Assemble single file"),
    _("Link object files to executable"),
    _("Archive files to library"),
    _("Generate HEX file"),
    _("Generate dependencies for file")
};

Compiler::Compiler(const wxString& name, const wxString& ID, const wxString& parentID) :
    m_Name(name),
    m_MacroListArguments(_T("")),
    m_ID(ID.Lower()),
    m_ParentID(parentID.Lower()),
    m_Valid(false),
    m_NeedValidityCheck(true),
    m_Mirrored(false)
{
    //ctor
    MakeValidID();

    m_Switches.supportsPCH = false;
    m_Switches.forceFwdSlashes = false;
    m_VersionString = wxEmptyString;

#if wxCHECK_VERSION(2, 9, 0)
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Added compiler \"%s\""), m_Name.wx_str()));
#else
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Added compiler \"%s\""), m_Name.c_str()));
#endif
}

Compiler::~Compiler()
{
    //dtor
}

bool Compiler::IsValid()
{
    if (!m_NeedValidityCheck)
        return m_Valid;

    if (GetMasterPath().IsEmpty())
        return true; // still initializing, don't try to test now

    m_NeedValidityCheck = false;

    wxString tmp = GetMasterPath() + _T("/") + m_Programs.C;
    m_Valid = wxFileExists(tmp);
    if (!m_Valid)
    {
        // look in extra paths too
        for (size_t i = 0; i < m_ExtraPaths.GetCount(); ++i)
        {
            tmp = m_ExtraPaths[i] + _T("/") + m_Programs.C;
            Manager::Get()->GetMacrosManager()->ReplaceMacros(tmp);
            m_Valid = wxFileExists(tmp);
            if (m_Valid)
                break;
        }
    }
    return m_Valid;
}

void Compiler::MakeValidID()
{
    // basically, make it XML-element compatible
    // only allow a-z, 0-9 and _resource_output
    // (it is already lowercase)
    // any non-conformant character will be removed

    wxString newID;
    if (m_ID.IsEmpty())
        m_ID = m_Name;

    size_t pos = 0;
    while (pos < m_ID.Length())
    {
        wxChar ch = m_ID[pos];
        if (wxIsalnum(ch) || ch == _T('_')) // valid character
            newID.Append(ch);
        else if (wxIsspace(ch)) // convert spaces to underscores
            newID.Append(_T('_'));
        ++pos;
    }

    // make sure it's not starting with a number.
    // if it is, prepend "cb"
    if (wxIsdigit(newID.GetChar(0)))
        newID.Prepend(_T("cb"));

    if (newID.IsEmpty()) // empty? wtf?
        cbThrow(_T("Can't create a valid compiler ID for ") + m_Name);
    m_ID = newID.Lower();

    // check for unique ID
    if (!IsUniqueID(m_ID))
        cbThrow(_T("Compiler ID already exists for ") + m_Name);
    m_CompilerIDs.Add(m_ID);
}


const wxString& Compiler::GetCommand(CommandType ct, const wxString& fileExtension) const
{
    size_t catchAll = 0;
    const CompilerToolsVector& vec = m_Commands[ct];

    if (!fileExtension.IsEmpty())
    {
        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (vec[i].extensions.GetCount() == 0)
            {
                catchAll = i;
                continue;
            }
            for (size_t n = 0; n < vec[i].extensions.GetCount(); ++n)
            {
                if(!m_Switches.caseSensitiveExt )
                {
                    if (vec[i].extensions[n].Lower() == fileExtension.Lower())
                        return vec[i].command;
                }
                else
                {
                    if (vec[i].extensions[n] == fileExtension)
                        return vec[i].command;
                }
            }
        }
    }
    return vec[catchAll].command;
}


const wxString& Compiler::GetCommand( const wxString& fileExtension) const
{
    static const wxString noValidReturn = wxEmptyString;
    int ct = 0;

    do
    {
        const CompilerToolsVector& vec = m_Commands[ct];

        if (!fileExtension.IsEmpty())
        {
            for (size_t i = 0; i < vec.size(); ++i)
            {
                if (vec[i].extensions.GetCount() == 0)
                {
                    continue;
                }
                for (size_t n = 0; n < vec[i].extensions.GetCount(); ++n)
                {
                    if(!m_Switches.caseSensitiveExt )
                    {
                        if (vec[i].extensions[n].Lower() == fileExtension.Lower())
                            return vec[i].command;
                    }
                    else
                    {
                        if (vec[i].extensions[n] == fileExtension)
                            return vec[i].command;
                    }
                }
            }
        }
    }
    while (++ct < ctCount);

    return noValidReturn;
}

const CompilerTool& Compiler::GetCompilerTool(CommandType ct, const wxString& fileExtension) const
{
    size_t catchAll = 0;
    const CompilerToolsVector& vec = m_Commands[ct];

    if (!fileExtension.IsEmpty())
    {
        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (vec[i].extensions.GetCount() == 0)
            {
                catchAll = i;
                continue;
            }
            for (size_t n = 0; n < vec[i].extensions.GetCount(); ++n)
            {
                if(!m_Switches.caseSensitiveExt )
                {
                    if (vec[i].extensions[n].Lower() == fileExtension.Lower())
                        return vec[i];
                }
                else
                {
                    if (vec[i].extensions[n] == fileExtension)
                        return vec[i];
                }
            }
        }
    }
    return vec[catchAll];
}


const CompilerTool& Compiler::GetCompilerTool(const wxString& fileExtension, int* resultPtr /*= NULL*/ ) const
{
    const CompilerToolsVector& vecEmpty = m_Commands[0];
    size_t catchAll = 0;
    int ct = 0;

    if (fileExtension.IsEmpty())
    {
        if(resultPtr)
            *resultPtr = -1;
        return vecEmpty[catchAll];
    }

    do
    {
        const CompilerToolsVector& vec = m_Commands[ct];

        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (vec[i].extensions.GetCount() == 0)
            {
                catchAll = i;
                continue;
            }
            for (size_t n = 0; n < vec[i].extensions.GetCount(); ++n)
            {
                wxString toolExt = vec[i].extensions[n];

                if(!m_Switches.caseSensitiveExt )
                {
                    if (toolExt.Lower() == fileExtension.Lower())
                    {
                        if(resultPtr)
                            *resultPtr = 0;
                        return vec[i];
                    }
                }
                else
                {
                    if (toolExt == fileExtension)
                    {
                        if(resultPtr)
                            *resultPtr = 0;
                        return vec[i];
                    }
                }
            }
        }
    }
    while (++ct < ctCount);

    if(resultPtr)
        *resultPtr = -1;
    return vecEmpty[catchAll];
}

void Compiler::MirrorCurrentSettings()
{
    // run just once
    if (m_Mirrored)
        return;

    // keep the current settings safe
    // so we can compare them when saving: this way we can only save what's
    // different from the defaults

    m_Mirror.Name             = m_Name;
    m_Mirror.MasterPath       = m_MasterPath;
    m_Mirror.RealMasterPath   = m_RealMasterPath;
    m_Mirror.ExtraPaths       = m_ExtraPaths;
    for (int i = 0; i < ctCount; ++i)
        m_Mirror.Commands[i]  = m_Commands[i];
    m_Mirror.Programs         = m_Programs;
    m_Mirror.Switches         = m_Switches;
    m_Mirror.DeviceOptionsOpt = m_DeviceOptions;
    m_Mirror.CompilerOptionsOpt  = m_CompilerOptions;
    m_Mirror.AssemblerOptionsOpt = m_AssemblerOptions;
    m_Mirror.LinkerOptionsOpt = m_LinkerOptions;
    m_Mirror.RegExes          = m_RegExes;

    m_Mirror.DeviceOptionsStr = m_DeviceOptionsStr;
    m_Mirror.CompilerOptionsStr = m_CompilerOptionsStr;
    m_Mirror.AssemblerOptionsStr = m_AssemblerOptionsStr;
    m_Mirror.LinkerOptionsStr = m_LinkerOptionsStr;
    m_Mirror.IncludeDirsStr   = MakeUniqueArray(m_IncludeDirsStr,    true);
    m_Mirror.LibDirsStr       = MakeUniqueArray(m_LibDirsStr,        true);
    m_Mirror.LinkLibsStr      = m_LinkLibsStr;
    m_Mirror.CmdsBeforeStr    = m_CmdsBeforeStr;
    m_Mirror.CmdsAfterStr     = m_CmdsAfterStr;

    m_Mirrored                = true;
}

void Compiler::SaveSettings(const wxString& baseKey)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("compiler"));

    // save settings version
    cfg->Write(_T("settings_version"), CompilerSettingsVersion);

    wxString tmp;

    tmp.Printf(_T("%s/%s"), baseKey.c_str(), m_ID.c_str());

    cfg->Write(tmp + _T("/name"),   m_Name);
    cfg->Write(tmp + _T("/parent"), m_ParentID, true);

    if (m_Mirror.DeviceOptionsStr != m_DeviceOptionsStr)
    {
        wxString key = GetStringFromArray(m_DeviceOptionsStr);
        cfg->Write(tmp + _T("/device_options"), key, false);
    }
    if (m_Mirror.CompilerOptionsStr != m_CompilerOptionsStr)
    {
        wxString key = GetStringFromArray(m_CompilerOptionsStr);
        cfg->Write(tmp + _T("/compiler_options"), key, false);
    }
    if (m_Mirror.AssemblerOptionsStr != m_AssemblerOptionsStr)
    {
        wxString key = GetStringFromArray(m_AssemblerOptionsStr);
        cfg->Write(tmp + _T("/compiler_options"), key, false);
    }
    if (m_Mirror.LinkerOptionsStr != m_LinkerOptionsStr)
    {
        wxString key = GetStringFromArray(m_LinkerOptionsStr);
        cfg->Write(tmp + _T("/linker_options"),   key, false);
    }
    if (m_Mirror.IncludeDirsStr != m_IncludeDirsStr)
    {
        wxString key = GetStringFromArray( MakeUniqueArray(m_IncludeDirsStr, true) );
        cfg->Write(tmp + _T("/include_dirs"),     key, false);
    }
    if (m_Mirror.LibDirsStr != m_LibDirsStr)
    {
        wxString key = GetStringFromArray( MakeUniqueArray(m_LibDirsStr, true) );
        cfg->Write(tmp + _T("/library_dirs"),     key, false);
    }
    if (m_Mirror.LinkLibsStr != m_LinkLibsStr)
    {
        wxString key = GetStringFromArray(m_LinkLibsStr);
        cfg->Write(tmp + _T("/libraries"),        key, false);
    }
    if (m_Mirror.CmdsBeforeStr != m_CmdsBeforeStr)
    {
        wxString key = GetStringFromArray(m_CmdsBeforeStr);
        cfg->Write(tmp + _T("/commands_before"),  key, true);
    }
    if (m_Mirror.CmdsAfterStr != m_CmdsAfterStr)
    {
        wxString key = GetStringFromArray(m_CmdsAfterStr);
        cfg->Write(tmp + _T("/commands_after"),   key, true);
    }

    if (m_Mirror.MasterPath != m_MasterPath)
        cfg->Write(tmp + _T("/master_path"),     GetMasterPath(false), true);
    if (m_Mirror.ExtraPaths != m_ExtraPaths)
        cfg->Write(tmp + _T("/extra_paths"),     GetStringFromArray( MakeUniqueArray(m_ExtraPaths, true), _T(";") ), true);
    if (m_Mirror.Programs.C != m_Programs.C)
        cfg->Write(tmp + _T("/c_compiler"),      m_Programs.C,         true);
    if (m_Mirror.Programs.CPP != m_Programs.CPP)
        cfg->Write(tmp + _T("/cpp_compiler"),      m_Programs.CPP,         true);
    if (m_Mirror.Programs.HEX != m_Programs.HEX)
        cfg->Write(tmp + _T("/hex_convertor"),    m_Programs.HEX,      true);
    if (m_Mirror.Programs.LD != m_Programs.LD)
        cfg->Write(tmp + _T("/linker"),          m_Programs.LD,        true);
    if (m_Mirror.Programs.LIB != m_Programs.LIB)
        cfg->Write(tmp + _T("/lib_linker"),      m_Programs.LIB,       true);
    if (m_Mirror.Programs.ASM != m_Programs.ASM)
        cfg->Write(tmp + _T("/assembler"),    m_Programs.ASM,   true);

    for (int i = 0; i < ctCount; ++i)
    {
        for (size_t n = 0; n < m_Commands[i].size(); ++n)
        {
            if (n >= m_Mirror.Commands[i].size() || m_Mirror.Commands[i][n] != m_Commands[i][n])
            {
                wxString key = wxString::Format(_T("%s/macros/%s/tool%d/"), tmp.c_str(), CommandTypeDescriptions[i].c_str(), n);
                cfg->Write(key + _T("command"), m_Commands[i][n].command);
                cfg->Write(key + _T("extensions"), m_Commands[i][n].extensions);
                cfg->Write(key + _T("generatedFiles"), m_Commands[i][n].generatedFiles);
            }
        }
    }

    // switches
    if (m_Mirror.Switches.includeDirs != m_Switches.includeDirs)
        cfg->Write(tmp + _T("/switches/includes"),                m_Switches.includeDirs,     true);
    if (m_Mirror.Switches.libDirs != m_Switches.libDirs)
        cfg->Write(tmp + _T("/switches/libs"),                    m_Switches.libDirs,         true);
    if (m_Mirror.Switches.linkLibs != m_Switches.linkLibs)
        cfg->Write(tmp + _T("/switches/link"),                    m_Switches.linkLibs,        true);
    if (m_Mirror.Switches.defines != m_Switches.defines)
        cfg->Write(tmp + _T("/switches/define"),                  m_Switches.defines,         true);
    if (m_Mirror.Switches.lnkdef != m_Switches.lnkdef)
        cfg->Write(tmp + _T("/switches/lnkdef"),                 m_Switches.lnkdef,   true);
    if (m_Mirror.Switches.objectExtension != m_Switches.objectExtension)
        cfg->Write(tmp + _T("/switches/objectext"),               m_Switches.objectExtension, true);
    if (m_Mirror.Switches.needDependencies != m_Switches.needDependencies)
        cfg->Write(tmp + _T("/switches/deps"),                    m_Switches.needDependencies);
    if (m_Mirror.Switches.forceCompilerUseQuotes != m_Switches.forceCompilerUseQuotes)
        cfg->Write(tmp + _T("/switches/forceCompilerQuotes"),     m_Switches.forceCompilerUseQuotes);
    if (m_Mirror.Switches.forceLinkerUseQuotes != m_Switches.forceLinkerUseQuotes)
        cfg->Write(tmp + _T("/switches/forceLinkerQuotes"),       m_Switches.forceLinkerUseQuotes);
    if (m_Mirror.Switches.logging != m_Switches.logging)
        cfg->Write(tmp + _T("/switches/logging"),                 m_Switches.logging);
    if (m_Mirror.Switches.libPrefix != m_Switches.libPrefix)
        cfg->Write(tmp + _T("/switches/libPrefix"),               m_Switches.libPrefix,       true);

    if (m_Mirror.Switches.libExtension != m_Switches.libExtension)
        cfg->Write(tmp + _T("/switches/libExtension"),            m_Switches.libExtension,    true);

//XXXXXXXXXX
    if (m_Mirror.Switches.execExtension != m_Switches.execExtension)
        cfg->Write(tmp + _T("/switches/execExtension"),            m_Switches.execExtension,    true);
    if (m_Mirror.Switches.mapExtension  != m_Switches.mapExtension )
        cfg->Write(tmp + _T("/switches/mapExtension "),            m_Switches.mapExtension ,    true);
    if (m_Mirror.Switches.hexExtension  != m_Switches.hexExtension )
        cfg->Write(tmp + _T("/switches/haxExtension "),            m_Switches.hexExtension ,    true);

    if (m_Mirror.Switches.linkerNeedsLibPrefix != m_Switches.linkerNeedsLibPrefix)
        cfg->Write(tmp + _T("/switches/linkerNeedsLibPrefix"),    m_Switches.linkerNeedsLibPrefix);
    if (m_Mirror.Switches.linkerNeedsLibExtension != m_Switches.linkerNeedsLibExtension)
        cfg->Write(tmp + _T("/switches/linkerNeedsLibExtension"), m_Switches.linkerNeedsLibExtension);
    if (m_Mirror.Switches.forceFwdSlashes != m_Switches.forceFwdSlashes)
        cfg->Write(tmp + _T("/switches/forceFwdSlashes"),         m_Switches.forceFwdSlashes);
    if (m_Mirror.Switches.supportsPCH != m_Switches.supportsPCH)
        cfg->Write(tmp + _T("/switches/supportsPCH"),             m_Switches.supportsPCH);
    if (m_Mirror.Switches.PCHExtension != m_Switches.PCHExtension)
        cfg->Write(tmp + _T("/switches/pchExtension"),            m_Switches.PCHExtension);
    if (m_Mirror.Switches.UseFlatObjects != m_Switches.UseFlatObjects)
        cfg->Write(tmp + _T("/switches/UseFlatObjects"),          m_Switches.UseFlatObjects);
    if (m_Mirror.Switches.UseFullSourcePaths != m_Switches.UseFullSourcePaths)
        cfg->Write(tmp + _T("/switches/UseFullSourcePaths"),      m_Switches.UseFullSourcePaths);
    if (m_Mirror.Switches.Use83Paths != m_Switches.Use83Paths)
        cfg->Write(tmp + _T("/switches/Use83Paths"),              m_Switches.Use83Paths);

    // regexes
    cfg->DeleteSubPath(tmp + _T("/regex"));
    wxString group;
    for (size_t i = 0; i < m_RegExes.Count(); ++i)
    {
        if (i < m_Mirror.RegExes.GetCount() && m_Mirror.RegExes[i] == m_RegExes[i])
            continue;

        group.Printf(_T("%s/regex/re%3.3d"), tmp.c_str(), i + 1);
        RegExStruct& rs = m_RegExes[i];
        cfg->Write(group + _T("/description"),  rs.desc,  true);
        if (rs.lt != 0)
            cfg->Write(group + _T("/type"),     rs.lt);
        cfg->Write(group + _T("/regex"),        rs.regex, true);
        if (rs.msg[0] != 0)
            cfg->Write(group + _T("/msg1"),     rs.msg[0]);
        if (rs.msg[1] != 0)
            cfg->Write(group + _T("/msg2"),     rs.msg[1]);
        if (rs.msg[2] != 0)
            cfg->Write(group + _T("/msg3"),     rs.msg[2]);
        if (rs.filename != 0)
            cfg->Write(group + _T("/filename"), rs.filename);
        if (rs.line != 0)
            cfg->Write(group + _T("/line"),     rs.line);
    }

    // custom vars
    wxString configpath = tmp + _T("/custom_variables/");
    cfg->DeleteSubPath(configpath);
    const StringHash& v = GetAllVars();
    for (StringHash::const_iterator it = v.begin(); it != v.end(); ++it)
        cfg->Write(configpath + it->first, it->second);
}

void Compiler::LoadSettings(const wxString& baseKey)
{
    // before loading any compiler settings, keep the current settings safe
    // so we can compare them when saving: this way we can only save what's
    // different from the defaults
    MirrorCurrentSettings();

    ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("compiler"));

    // read settings version
    wxString version = cfg->Read(_T("settings_version"));
    bool versionMismatch = version != CompilerSettingsVersion;

    wxString tmp;
    tmp.Printf(_T("%s/%s"), baseKey.c_str(), m_ID.c_str());

    if (!cfg->Exists(tmp + _T("/name")))
        return;

    wxString sep = wxFileName::GetPathSeparator();

    //  m_Name = cfg->Read(tmp + _T("/name"), m_Name);

    SetMasterPath( cfg->Read(tmp + _T("/master_path"), GetMasterPath(false)));
    m_ExtraPaths         = MakeUniqueArray(GetArrayFromString(cfg->Read(tmp + _T("/extra_paths"), _T("")), _T(";")), true);
    m_Programs.C         = cfg->Read(tmp + _T("/c_compiler"),      m_Programs.C);
    m_Programs.CPP       = cfg->Read(tmp + _T("/cpp_compiler"),    m_Programs.CPP);
    m_Programs.HEX       = cfg->Read(tmp + _T("/hex_convertor"),   m_Programs.HEX);
    m_Programs.LD        = cfg->Read(tmp + _T("/linker"),          m_Programs.LD);
    m_Programs.LIB       = cfg->Read(tmp + _T("/lib_linker"),      m_Programs.LIB);
    m_Programs.ASM       = cfg->Read(tmp + _T("/assembler"),       m_Programs.ASM);

    // set member variable containing the version string with the configurated toolchain executables, not only
    // with the default ones, otherwise we might have an empty version-string
    // Some MinGW installations do not includee "mingw32-gcc" !!
    SetVersionString();

    SetCompilerStrOptions (GetArrayFromString(cfg->Read(tmp + _T("/compiler_options"),  wxEmptyString)));
    SetAssemblerStrOptions(GetArrayFromString(cfg->Read(tmp + _T("/assembler_options"), wxEmptyString)));
    SetLinkerStrOptions   (GetArrayFromString(cfg->Read(tmp + _T("/linker_options"),    wxEmptyString)));
    SetIncludeDirs        (GetArrayFromString(cfg->Read(tmp + _T("/include_dirs"),      wxEmptyString)));
    SetLibDirs            (GetArrayFromString(cfg->Read(tmp + _T("/library_dirs"),      wxEmptyString)));
    SetLinkLibs           (GetArrayFromString(cfg->Read(tmp + _T("/libraries"),         wxEmptyString)));
    SetCommandsBeforeBuild(GetArrayFromString(cfg->Read(tmp + _T("/commands_before"),   wxEmptyString)));
    SetCommandsAfterBuild (GetArrayFromString(cfg->Read(tmp + _T("/commands_after"),    wxEmptyString)));

    for (int i = 0; i < ctCount; ++i)
    {
        wxArrayString keys = cfg->EnumerateSubPaths(tmp + _T("/macros/") + CommandTypeDescriptions[i]);
        for (size_t n = 0; n < keys.size(); ++n)
        {
            unsigned long index = 0;
            if (keys[n].Mid(4).ToULong(&index)) // skip 'tool'
            {
                while (index >= m_Commands[i].size())
                    m_Commands[i].push_back(CompilerTool());
                CompilerTool& tool = m_Commands[i][index];

                wxString key        = wxString::Format(_T("%s/macros/%s/tool%lu/"), tmp.c_str(), CommandTypeDescriptions[i].c_str(), index);
                tool.command        = cfg->Read(key + _T("command"));
                tool.extensions     = cfg->ReadArrayString(key + _T("extensions"));
                tool.generatedFiles = cfg->ReadArrayString(key + _T("generatedFiles"));
            }
        }
    }

    // switches
    m_Switches.includeDirs             = cfg->Read(tmp + _T("/switches/includes"),                    m_Switches.includeDirs);
    m_Switches.libDirs                 = cfg->Read(tmp + _T("/switches/libs"),                        m_Switches.libDirs);
    m_Switches.linkLibs                = cfg->Read(tmp + _T("/switches/link"),                        m_Switches.linkLibs);
    m_Switches.defines                 = cfg->Read(tmp + _T("/switches/define"),                      m_Switches.defines);
    m_Switches.lnkdef                  = cfg->Read(tmp + _T("/switches/lnkdef"),                      m_Switches.lnkdef);
    m_Switches.objectExtension         = cfg->Read(tmp + _T("/switches/objectext"),                   m_Switches.objectExtension);

    m_Switches.needDependencies        = cfg->ReadBool(tmp + _T("/switches/deps"),                    m_Switches.needDependencies);
    m_Switches.forceCompilerUseQuotes  = cfg->ReadBool(tmp + _T("/switches/forceCompilerQuotes"),     m_Switches.forceCompilerUseQuotes);
    m_Switches.forceLinkerUseQuotes    = cfg->ReadBool(tmp + _T("/switches/forceLinkerQuotes"),       m_Switches.forceLinkerUseQuotes);
    m_Switches.logging = (CompilerLoggingType)cfg->ReadInt(tmp + _T("/switches/logging"),             m_Switches.logging);
    m_Switches.libPrefix               = cfg->Read(tmp + _T("/switches/libPrefix"),                   m_Switches.libPrefix);

    m_Switches.execExtension           = cfg->Read(tmp + _T("/switches/execExtension"),               m_Switches.execExtension);
    m_Switches.mapExtension            = cfg->Read(tmp + _T("/switches/mapExtension"),                m_Switches.mapExtension);
    m_Switches.hexExtension            = cfg->Read(tmp + _T("/switches/hexExtension"),                m_Switches.hexExtension);

    m_Switches.libExtension            = cfg->Read(tmp + _T("/switches/libExtension"),                m_Switches.libExtension);
    m_Switches.linkerNeedsLibPrefix    = cfg->ReadBool(tmp + _T("/switches/linkerNeedsLibPrefix"),    m_Switches.linkerNeedsLibPrefix);
    m_Switches.linkerNeedsLibExtension = cfg->ReadBool(tmp + _T("/switches/linkerNeedsLibExtension"), m_Switches.linkerNeedsLibExtension);
    m_Switches.forceFwdSlashes         = cfg->ReadBool(tmp + _T("/switches/forceFwdSlashes"),         m_Switches.forceFwdSlashes);
    m_Switches.supportsPCH             = cfg->ReadBool(tmp + _T("/switches/supportsPCH"),             m_Switches.supportsPCH);
    m_Switches.PCHExtension            = cfg->Read(tmp + _T("/switches/pchExtension"),                m_Switches.PCHExtension);
    m_Switches.UseFlatObjects          = cfg->ReadBool(tmp + _T("/switches/UseFlatObjects"),          m_Switches.UseFlatObjects);
    m_Switches.UseFullSourcePaths      = cfg->ReadBool(tmp + _T("/switches/UseFullSourcePaths"),      m_Switches.UseFullSourcePaths);
    m_Switches.Use83Paths              = cfg->ReadBool(tmp + _T("/switches/Use83Paths"),              m_Switches.Use83Paths);

    // regexes

    // because we 're only saving changed regexes, we can't just iterate like before.
    // instead, we must iterate all child-keys and deduce the regex index number from
    // the key name
    wxArrayString keys = cfg->EnumerateSubPaths(tmp + _T("/regex/"));
    wxString group;
    long index = 0;
    for (size_t i = 0; i < keys.GetCount(); ++i)
    {
        wxString key = keys[i];

        // reNNN
        if (!key.StartsWith(_T("re")))
            continue;
        key.Remove(0, 2);
        if (!key.ToLong(&index, 10))
            continue;

        // 'index' now holds the regex index.
        // read everything and either assign it to an existing regex
        // if the index exists, or add a new regex

        group.Printf(_T("%s/regex/re%3.3ld"), tmp.c_str(), index);
        if (!cfg->Exists(group+_T("/description")))
            continue;

        RegExStruct rs;
        rs.desc     = cfg->Read(group + _T("/description"));
        rs.lt       = (CompilerLineType)cfg->ReadInt(group + _T("/type"), 0);
        rs.regex    = cfg->Read(group + _T("/regex"));
        rs.msg[0  ] = cfg->ReadInt(group + _T("/msg1"), 0);
        rs.msg[1]   = cfg->ReadInt(group + _T("/msg2"), 0);
        rs.msg[2]   = cfg->ReadInt(group + _T("/msg3"), 0);
        rs.filename = cfg->ReadInt(group + _T("/filename"), 0);
        rs.line     = cfg->ReadInt(group + _T("/line"), 0);

        if (index <= (long)m_RegExes.GetCount())
            m_RegExes[index - 1] = rs;
        else
            m_RegExes.Add(rs);
    }

    // custom vars
    wxString configpath = tmp + _T("/custom_variables/");
    UnsetAllVars();
    wxArrayString list = cfg->EnumerateKeys(configpath);
    for (unsigned int i = 0; i < list.GetCount(); ++i)
        SetVar(list[i], cfg->Read(configpath + _T('/') + list[i]), false);

    if (versionMismatch)
    {
        wxString msg;
        msg << _("Some compiler settings defaults have changed in this version.\n"
                 "It is recommended that you allow updating of your settings to the new defaults.\n"
                 "Only disallow this if you don't want to lose any customizations you have done to this compiler's settings.\n\n"
                 "Note that the only settings that are affected are those found in \"Advanced compiler options\"...\n\n"
                 "Do you want to update your current settings to the new defaults?");
        // don't ask if the compiler is not valid (i.e. not installed), just update
        if (!IsValid() || cbMessageBox(msg, m_Name, wxICON_QUESTION | wxYES_NO) == wxID_YES)
        {
            for (int i = 0; i < ctCount; ++i)
                m_Commands[i] = m_Mirror.Commands[i];
            m_Switches          = m_Mirror.Switches;
            m_DeviceOptions     = m_Mirror.DeviceOptionsOpt;
            m_CompilerOptions   = m_Mirror.CompilerOptionsOpt;
            m_AssemblerOptions  = m_Mirror.AssemblerOptionsOpt;
            m_LinkerOptions     = m_Mirror.LinkerOptionsOpt;
            m_RegExes           = m_Mirror.RegExes;
        }
    }
}

CompilerLineType Compiler::CheckForWarningsAndErrors(const wxString& line)
{
    m_ErrorFilename.Clear();
    m_ErrorLine.Clear();
    m_Error.Clear();

    for (size_t i = 0; i < m_RegExes.Count(); ++i)
    {
        RegExStruct& rs = m_RegExes[i];
        if (rs.regex.IsEmpty())
            continue;
        wxRegEx regex(rs.regex);
        if (regex.Matches(line))
        {
            if (rs.filename > 0)
                m_ErrorFilename = UnixFilename(regex.GetMatch(line, rs.filename));
            if (rs.line > 0)
                m_ErrorLine = regex.GetMatch(line, rs.line);
            for (int x = 0; x < 3; ++x)
            {
                if (rs.msg[x] > 0)
                {
                    if (!m_Error.IsEmpty())
                        m_Error << _T(" ");
                    m_Error << regex.GetMatch(line, rs.msg[x]);
                }
            }
            return rs.lt;
        }
    }
    return cltNormal; // default return value
}
