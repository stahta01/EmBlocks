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

#ifndef COMPILEOPTIONSBASE_H
#define COMPILEOPTIONSBASE_H

#include "globals.h"
#include <wx/hashmap.h>

WX_DECLARE_STRING_HASH_MAP(wxString, StringHash);


// Possible modes for the pre/post build run steps
enum PrePostBuildMode {
  DIS_RUN     = 0,
  ALWAYS_RUN  = 1,
  ONBUILD_RUN = 2
};

/**
  * This is a base class for all classes needing compilation parameters. It
  * offers functions to get/set the following:\n
  * \li Compiler options
  * \li Linker options
  * \li Compiler include dirs
  * \li Linker include dirs
  * \li Custom commands to be executed before/after build
  * \li The settings modification status
  * \n\n
  * These settings are used by the compiler plugins to construct the necessary
  * compilation commands.
  */
class DLLIMPORT CompileOptionsBase
{
    public:
        CompileOptionsBase();
        virtual ~CompileOptionsBase();

        // Make C::B plugins happy if they need it, we support all platforms because we don't support it ;-)
        virtual bool SupportsCurrentPlatform() {return true;};

        virtual void SetLinkerStrOptions(const wxArrayString& linkerOpts);
        virtual const wxArrayString& GetLinkerStrOptions() const;
        virtual void AddLinkerStrOption(const wxString& option);
        virtual void RemoveLinkerStrOption(const wxString& option);

        virtual void SetLinkLibs(const wxArrayString& linkLibs);
        virtual const wxArrayString& GetLinkLibs() const;
        virtual void AddLinkLib(const wxString& lib);
        virtual void RemoveLinkLib(const wxString& lib);

        virtual void SetCompilerStrOptions(const wxArrayString& compilerOpts);
        virtual const wxArrayString& GetCompilerStrOptions() const;
        virtual void AddCompilerStrOption(const wxString& option);
        virtual void RemoveCompilerStrOption(const wxString& option);

        virtual void SetDeviceStrOptions(const wxArrayString& deviceOpts);
        virtual const wxArrayString& GetDeviceStrOptions() const;
        virtual void AddDeviceStrOption(const wxString& option);
        virtual void RemoveDeviceStrOption(const wxString& option);

        virtual void SetAssemblerStrOptions(const wxArrayString& assemblerOpts);
        virtual const wxArrayString& GetAssemblerStrOptions() const;
        virtual void AddAssemblerStrOption(const wxString& option);
        virtual void RemoveAssemblerStrOption(const wxString& option);

        virtual void SetIncludeDirs(const wxArrayString& includeDirs);
        virtual const wxArrayString& GetIncludeDirs() const;
        virtual void AddIncludeDir(const wxString& option);
        virtual void RemoveIncludeDir(const wxString& option);

        virtual void SetLibDirs(const wxArrayString& libDirs);
        virtual const wxArrayString& GetLibDirs() const;
        virtual void AddLibDir(const wxString& option);
        virtual void RemoveLibDir(const wxString& option);

        virtual void SetCommandsBeforeBuild(const wxArrayString& commands);
        virtual const wxArrayString& GetCommandsBeforeBuild() const;
        virtual void AddCommandsBeforeBuild(const wxString& command);
        virtual void RemoveCommandsBeforeBuild(const wxString& command);

        virtual void SetCommandsAfterBuild(const wxArrayString& commands);
        virtual const wxArrayString& GetCommandsAfterBuild() const;
        virtual void AddCommandsAfterBuild(const wxString& command);
        virtual void RemoveCommandsAfterBuild(const wxString& command);

        virtual void SetBuildScripts(const wxArrayString& scripts);
        virtual const wxArrayString& GetBuildScripts() const;
        virtual void AddBuildScript(const wxString& script);
        virtual void RemoveBuildScript(const wxString& script);

        virtual bool GetModified() const;
        virtual void SetModified(bool modified);
        virtual int  GetPreBuildRunSettings() { return (int)m_PreBuildRunMode; };
        virtual int  GetPostBuildRunSettings(){ return (int)m_PostBuildRunMode; };
        virtual void SetPreBuildRunSettings(int mode);
        virtual void SetPostBuildRunSettings(int mode);
        virtual bool SetVar(const wxString& key, const wxString& value, bool onlyIfExists = false);
        virtual bool UnsetVar(const wxString& key);
        virtual void UnsetAllVars();
        virtual const wxString& GetVar(const wxString& key) const;
        virtual const StringHash& GetAllVars() const;
    protected:
        wxArrayString m_DeviceOptionsStr;
        wxArrayString m_CompilerOptionsStr;
        wxArrayString m_AssemblerOptionsStr;
        wxArrayString m_LinkerOptionsStr;
        wxArrayString m_LinkLibsStr;
        wxArrayString m_IncludeDirsStr;
        wxArrayString m_LibDirsStr;
        wxArrayString m_CmdsBeforeStr;
        wxArrayString m_CmdsAfterStr;
        wxArrayString m_ScriptsStr;
        bool m_Modified;
        PrePostBuildMode m_PreBuildRunMode;
        PrePostBuildMode m_PostBuildRunMode;
        StringHash m_Vars;
    private:
};

#endif // COMPILEOPTIONSBASE_H
