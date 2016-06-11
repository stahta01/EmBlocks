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

#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <map>

#include <wx/string.h>
#include <wx/filename.h>
#include <wx/dynarray.h>
#include <macrosmanager.h>
#include "compileoptionsbase.h"
#include "compileroptions.h"
#include "compilervaroptions.h"

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#endif /* ATTRIBUTE_UNUSED */

class CompilerCommandGenerator;
class cbProject;
class ProjectBuildTarget;
class ProjectFile;

/*
    Macros used in command specs:

    Compiler executable: $compiler
    Compiler CPP executable: $cppcompiler
    Resource compiler executable: $rescomp
    Linker executable: $linker
    Linker executable for static libs: $lib_linker
    Compiler options: $options
    Linker options: $link_options
    Include dirs: $includes
    Rsource include dirs: $res_includes
    Library dirs: $libdirs
    Link libraries: $libs
    Source file: $file
    Object file: $object
    Dependency result: $dep_object
    All *linkable* object files: $link_objects
    All *linkable* flat object files: $link_flat_objects
    output file: $output
    Static lib output file: $static_output
    Dynamic lib output file: $dynamic_output
    Dynamic lib DEF output file: $def_output
    Resources output file: $resource_output
    Objects output dir: $output_dir_objects

    Usual special chars apply: \t, \n, etc.
    The command output should be ready for inclusion
    in a Makefile (properly tab aligned but *no* tabs at the start of the string)
    The macro substitution happens in compiler's MakefileGenerator.
*/

/// Enum categorizing compiler's output line as warning/error/info/normal
enum CompilerLineType
{
    cltNormal = 0,
    cltWarning,
    cltError,
    cltInfo
};

// regexes array declaration
struct RegExStruct
{
    RegExStruct()
        : desc(_T("Unknown")), lt(cltError), regex(_T("")), filename(0), line(0)
    {
        memset(msg, 0, sizeof(msg));
    }
    RegExStruct(const RegExStruct& rhs)
        : desc(rhs.desc), lt(rhs.lt), regex(rhs.regex), filename(rhs.filename), line(rhs.line)
    {
        memcpy(msg, rhs.msg, sizeof(msg));
    }
    RegExStruct(const wxString&  _desc,
                CompilerLineType _lt,
                const wxString&  _regex,
                int              _msg,
                int              _filename = 0,
                int              _line     = 0,
                int              _msg2     = 0,
                int              _msg3     = 0)
        : desc(_desc), lt(_lt), regex(_regex), filename(_filename), line(_line)
    {
        msg[0] = _msg;
        msg[1] = _msg2;
        msg[2] = _msg3;
    }
    bool operator!=(const RegExStruct& other)
    {
        return !(*this == other);
    }
    bool operator==(const RegExStruct& other)
    {
        return (   desc     == other.desc
                && lt       == other.lt
                && regex    == other.regex
                && msg[0]   == other.msg[0]
                && msg[1]   == other.msg[1]
                && msg[2]   == other.msg[2]
                && filename == other.filename
                && line     == other.line );
    }
    wxString         desc;     // title of this regex
    CompilerLineType lt;       // classify the line, if regex matches
    wxString         regex;    // the regex to match
    int              msg[3];   // up-to 3 sub-expression nr for warning/error message
    int              filename; // sub-expression nr for filename
    int              line;     // sub-expression nr for line number
    // if more than one sub-expressions are entered for msg,
    // they are appended to each other, with one space in between.
    // Appending takes place in the same order...
};
WX_DECLARE_OBJARRAY(RegExStruct, RegExArray);

/// Helper enum to retrieve compiler commands
enum CommandType
{
    ctCompileObjectCmd = 0, ///< Compile object command, e.g. "$compiler $options $includes -c $file -o $object"
    ctCompileCppObjectCmd,   ///< Cpp compiler object command, e.g. "$cppcompiler $options $includes -c $file -o $object"
    ctAssembleObjectCmd,    ///< Assembleobject command, e.g. "$assembler $options $includes -c $file -o $object"
    ctLinkExeCmd,           ///< Link executable command, e.g. "$linker $libdirs -o $exe_output $link_objects $libs -mwindows"
    ctLinkLibraryCmd,       ///< Link dynamic (dll) lib command, e.g. "$linker -shared -Wl,--output-def=$def_output -Wl,--out-implib=$static_output -Wl,--dll $libdirs $link_objects $libs -o $dynamic_output"
    ctHexConvCmd,           ///< Create hex file command
    ctGenDependenciesCmd,   ///< Generate dependencies command
    ctCount                 ///< Do NOT use
};



/// Helper enum for type of compiler logging
enum CompilerLoggingType
{
    clogFull,
    clogSimple,
    clogNone
};

enum AutoDetectResult
{
    adrDetected,
    adrGuessed,
    adrUndefined
};

/// Struct to keep programs
struct CompilerPrograms
{
    wxString C;         // C compiler
    wxString CPP;       // C++ compiler (not used right now)
    wxString ASM;       // Assembler
    wxString LD;        // Linker
    wxString LIB;       // Library archiver
    wxString HEX;       // Hex convertor
};


/// Struct to keep switches
struct CompilerSwitches
{
    wxString includeDirs;         // -I
    wxString libDirs;             // -L
    wxString linkLibs;            // -l
    wxString defines;             // -D
    wxString macros;              // assembler
    wxString lnkdef;              // Linker -Wl,--defsym
    wxString objectExtension;     // o
    wxString execExtension;       // elf
    wxString mapExtension;        // map
    wxString hexExtension;        // hex
    bool forceFwdSlashes;         // force use forward slashes in file/path names (used by CompilerCommandGenerator)
    bool forceLinkerUseQuotes;    // use quotes for filenames in linker command line (needed or not)?
    bool forceCompilerUseQuotes;  // use quotes for filenames in compiler command line (needed or not)?
    bool needDependencies;        // true
    bool caseSensitiveExt;        // Are extensions case sensitive for command find
    CompilerLoggingType logging;  // clogFull
    wxString libPrefix;           // lib
    wxString libExtension;        // a
    bool linkerNeedsLibPrefix;    // when adding a link library, linker needs prefix?
    bool linkerNeedsLibExtension; // when adding a link library, linker needs extension?
    bool supportsPCH;             // supports pre-compiled headers?
    wxString PCHExtension;        // pre-compiled headers extension
    bool UseFlatObjects;          // Use Flat object file names (no extra subdirs)?
    bool UseFullSourcePaths;      // This is mainly a workaround for the GDB debugger, apparently I doesn't deal
                                  // well with relative paths, therefore for GCC it is better to specify the source
                                  // full to the compiler in a full path notation, for all other compilers it is
                                  // suggested to keep this switch at false
    bool Use83Paths;              // This is mainly a workaround for the resource compiler under Windows, apparently
                                  // it doesn't deal well with spaces in the (include) path even if the path is quoted,
                                  // therefore use 8.3 notation without spaces on Windows.
                                  // However, this will apply to all include path's as other tools might have the
                                  // same issue and it won't hurt to apply it to all include directories, if enabled.

    CompilerSwitches(); // constructor initializing the members, specific compilers should overrule if needed
};

/// Struct for compiler/linker commands
struct CompilerTool
{
    // extensions string will be converted to array by GetArrayFromString using DEFAULT_ARRAY_SEP (;)
    // as separator
    CompilerTool(const wxString& command = wxEmptyString, const wxString& extensions = wxEmptyString)
        : command(command), extensions(GetArrayFromString(extensions))
    {}
    CompilerTool(const CompilerTool& rhs)
        : command(rhs.command), extensions(rhs.extensions), generatedFiles(rhs.generatedFiles)
    {}
    bool operator==(const CompilerTool& rhs) const { return command == rhs.command && extensions == rhs.extensions && generatedFiles == rhs.generatedFiles; }
    bool operator!=(const CompilerTool& rhs) const { return !(*this == rhs); }

    wxString      command;        ///< command to execute
    wxArrayString extensions;     ///< file extensions for which the command will be invoked (no leading dot)
    wxArrayString generatedFiles; ///< the native language files this command generates that should be further compiled
};

typedef std::vector<CompilerTool> CompilerToolsVector;


/**
  * @brief Abstract base class for compilers.
  *
  * Create a derived class and add it in compilerfactory.cpp
  */
class DLLIMPORT Compiler : public CompileOptionsBase
{
    public:
        static const wxString FilePathWithSpaces;
        Compiler(const wxString& name, const wxString& ID, const wxString& parentID = wxEmptyString);
        virtual ~Compiler();

        /** @brief Check if the compiler is actually valid (installed). */
        virtual bool IsValid();
        /** @brief Get the arguments for the compiler to get the buildin macro list */
        virtual wxString GetMacroListArguments() {return m_MacroListArguments; }
        /** @brief Check if the supplied string is a compiler warning/error */
        virtual CompilerLineType CheckForWarningsAndErrors(const wxString& line);
        /** @brief Returns warning/error filename. Use it after a call to CheckForWarningsAndErrors() */
        virtual wxString GetLastErrorFilename(){ return m_ErrorFilename; }
        /** @brief Returns warning/error line number (as a string). Use it after a call to CheckForWarningsAndErrors() */
        virtual wxString GetLastErrorLine(){ return m_ErrorLine; }
        /** @brief Returns warning/error actual string. Use it after a call to CheckForWarningsAndErrors() */
        virtual wxString GetLastError(){ return m_Error; }
        /** @brief Get the compiler's name */
        virtual const wxString& GetName() const { return m_Name; }
        /** @brief Get the compiler's master path, if called with false the macro's will not be solved */
        virtual const wxString& GetMasterPath(bool realPath = true) const { return (realPath ? m_RealMasterPath: m_MasterPath); }
        /** @brief Get the compiler's extra paths */
        virtual const wxArrayString& GetExtraPaths() const { return m_ExtraPaths; }
        /** @brief Get the debugger which belongs to this compiler  */
        virtual wxString GetDebugger() { return _T(""); }
        /** @brief Get the compiler's programs */
        virtual const CompilerPrograms& GetPrograms() const { return m_Programs; }
        /** @brief Get the compiler's generic switches */
        virtual const CompilerSwitches& GetSwitches() const { return m_Switches; }
        /** @brief Get the compiler's options */
        virtual const CompilerOptions& GetCompilerOptions() const { return m_CompilerOptions; }
        /** @brief Get the assembler's options */
        virtual const CompilerOptions& GetAssemblerOptions() const { return m_AssemblerOptions; }
        /** @brief Get the variable options */
        virtual const CompilerVarOptions& GetDeviceOptions() const { return m_DeviceOptions; }

        /** @brief Get the inker's options */
        virtual const CompilerOptions& GetLinkCheckOptions() const { return m_LinkerOptions; }
        /** @brief Get a command based on CommandType
          * @param ct The command type to process
          * @param fileExtension the file's extension (no leading dot)
          */
        virtual const wxString& GetCommand(CommandType ct, const wxString& fileExtension = wxEmptyString) const;

        /** @brief Get a command based on file extension
          * @param fileExtension the file's extension (no leading dot)
          */
        virtual const wxString& GetCommand( const wxString& fileExtension ) const;

        /** @brief Get a compiler tool based on CommandType */
        virtual const CompilerTool& GetCompilerTool(CommandType ct, const wxString& fileExtension = wxEmptyString) const;

        /** @brief Get a compiler tool based on file extension */
        virtual const CompilerTool& GetCompilerTool(const wxString& fileExtension, int* resultPtr = NULL) const;

        /** @brief Get a command tool vector based on CommandType (used by advanced compiler dialog) */
        virtual CompilerToolsVector& GetCommandToolsVector(CommandType ct) { return m_Commands[ct]; }
        /** @brief Get the array of regexes used in errors/warnings recognition */
        virtual const RegExArray& GetRegExArray(){ return m_RegExes; }
        /** @brief Load the default (preset) array of regexes used in errors/warnings recognition */
        virtual void LoadDefaultRegExArray() = 0;

        /** @brief Set the compiler's name */
        virtual void SetName(const wxString& name){ m_Name = name; }
        /** @brief Set the compiler's master path (must contain "bin", "include" and "lib") */
        virtual void SetMasterPath(const wxString& path){ m_MasterPath = path; m_RealMasterPath = path;
                                                          Manager::Get()->GetMacrosManager()->ReplaceMacros(m_RealMasterPath); m_NeedValidityCheck = true; }
        /** @brief Set the compiler's extra paths */
        virtual void SetExtraPaths(const wxArrayString& paths){ m_ExtraPaths = paths; m_NeedValidityCheck = true; }
        /** @brief Set the compiler's programs */
        virtual void SetPrograms(const CompilerPrograms& programs){ m_Programs = programs; m_NeedValidityCheck = true; }
        /** @brief Set the compiler's generic switches */
        virtual void SetSwitches(const CompilerSwitches& switches){ m_Switches = switches; }
        /** @brief Set the device options */
        virtual void SetDeviceOptions(const CompilerVarOptions& options){ m_DeviceOptions = options; }
        /** @brief Set the compiler's options */
        virtual void SetCompilerOptions(const CompilerOptions& options){ m_CompilerOptions = options; }
        /** @brief Set the compiler's options */
        virtual void SetAssemblerOptions(const CompilerOptions& options){ m_AssemblerOptions = options; }
        /** @brief Set the array of regexes used in errors/warnings recognition */
        virtual void SetRegExArray(const RegExArray& regexes){ m_RegExes = regexes; }

        /** Initialize for use with the specified @c project.
          * Transfers the call to the generator returned by GetCommandGenerator()*/
        virtual void Init(cbProject* project) {};

        /** Get the command line to compile/link the specific file.
          * Transfers the call to the generator returned by GetCommandGenerator()*/
        virtual void GenerateCommandLine(wxString& macro,
                                        ProjectBuildTarget* target,
                                        ProjectFile* pf,
                                        const wxString& file,
                                        const wxString& object,
                                        const wxString& FlatObject,
                                        const wxString& deps)   {macro = _T("");}

        /** @brief Get the full include dirs used in the actuall command line.
          *
          * These are the actual include dirs that will be used for building
          * and might be different than target->GetIncludeDirs(). This is
          * because it's the sum of target include dirs + project include dirs +
          * build-script include dirs.
          * @note This is only valid after Init() has been called.
          */
        virtual const wxArrayString& GetCompilerSearchDirs(ProjectBuildTarget* target) {static wxArrayString retEmpty; retEmpty.Clear(); return retEmpty; }

        /** @brief Get the full linker dirs used in the actual command line.
          *
          * These are the actual linker dirs that will be used for building
          * and might be different than target->GetLibDirs(). This is
          * because it's the sum of target linker dirs + project linker dirs +
          * build-script linker dirs.
          * @note This is only valid after Init() has been called.
          */
        virtual const wxArrayString& GetLinkerSearchDirs(ProjectBuildTarget* target)  {static wxArrayString retEmpty; retEmpty.Clear(); return retEmpty; }

        /** @brief Save settings */
        virtual void SaveSettings(const wxString& baseKey);
        /** @brief Load settings */
        virtual void LoadSettings(const wxString& baseKey);
        /** @brief Reset settings to defaults.
          * Put initialization code here and call this from the default constructor.
          */
        virtual void Reset() = 0;
        /** @brief Try to auto-detect the compiler's installation directory */
        virtual AutoDetectResult AutoDetectInstallationDir() = 0;

        /** @brief Get this compiler's unique ID */
        const wxString& GetID() const { return m_ID; }
        /** @brief Get this compiler's parent's unique ID */
        const wxString& GetParentID() const { return m_ParentID; }

        /** @brief Get the command type descriptions (used in advanced compiler options) */
        static wxString CommandTypeDescriptions[ctCount];

        /** @brief Set the compiler version string. Please override this virtual function
          * with your own compiler-version detection code if you want to use this.
          *
          * By default this function does nothing. */
        virtual void SetVersionString() { return; };

        /** @brief Is called after that the job is done by directcommands.cpp */
        virtual void OnJobEnd( wxString outFile ATTRIBUTE_UNUSED, int exitCode ATTRIBUTE_UNUSED) { return; };

        /** @brief Get the compiler version string */
        const wxString GetVersionString() const { return m_VersionString; };

        /** @brief Get the lexer language which belongs to this file according the toolchain compiler
          *        return an empty string for default lexer handling.
          */
        virtual wxString GetLanguage (const wxString& filename) { return _T(""); };

        /** @brief Get the address width of the compilers architecture
          *
          */
        virtual int GetAddressWidth () { return 32; };

        /** @brief Is called just after that the command line is build and before the actual call of the tool
          */
        virtual void PreBuildStage(wxString&  macro, wxString& cfString, wxString& afString, wxString& lfString) {  };

    protected:
        friend class CompilerFactory;

        /** This is to be overriden,  compiler needs to set the
          * command line generator.
          */
        virtual CompilerCommandGenerator* GetCommandGenerator() {return 0;}

        // purposely non-virtual
        bool IsUniqueID(const wxString& ID){ return m_CompilerIDs.Index(ID) == wxNOT_FOUND; }
        // converts, if needed, m_ID to something that is valid
        void MakeValidID();

        // keeps a copy of current settings (works only the first time it's called)
        void MirrorCurrentSettings();

        // set the following members in your class
        wxString            m_Name;
        wxString            m_MasterPath;
        wxString            m_RealMasterPath;  // With processed macro's
        wxString            m_MacroListArguments;
        wxArrayString       m_ExtraPaths;
        CompilerToolsVector m_Commands[ctCount];
        CompilerPrograms    m_Programs;
        CompilerSwitches    m_Switches;
        CompilerVarOptions  m_DeviceOptions;
        CompilerOptions     m_CompilerOptions;
        CompilerOptions     m_AssemblerOptions;
        CompilerOptions     m_LinkerOptions;
        RegExArray          m_RegExes;
        wxString            m_ErrorFilename;
        wxString            m_ErrorLine;
        wxString            m_Error;
        wxString            m_VersionString;
    private:
        wxString m_ID;
        wxString m_ParentID; // -1 for builtin compilers, the builtin compiler's ID to derive from for user compilers...
        static wxArrayString m_CompilerIDs; // map to guarantee unique IDs
        bool m_Valid; // 'valid' flag
        bool m_NeedValidityCheck; // flag to re-check validity (raised when changing compiler paths)

        // "mirror" default settings for comparing when saving (to save only those that differ from defaults)
        struct MirrorSettings
        {
            wxString         Name;
            wxString         MasterPath;
            wxString         RealMasterPath;
            wxArrayString    ExtraPaths;
            CompilerPrograms Programs;

            // these are the CompileOptionsBase settings that each compiler keeps on a global level
            wxArrayString DeviceOptionsStr;
            wxArrayString CompilerOptionsStr;
            wxArrayString AssemblerOptionsStr;
            wxArrayString LinkerOptionsStr;
            wxArrayString IncludeDirsStr;
            wxArrayString ResIncludeDirs;
            wxArrayString LibDirsStr;
            wxArrayString LinkLibsStr;
            wxArrayString CmdsBeforeStr;
            wxArrayString CmdsAfterStr;

            // below are the settings that the user is asked to revert to defaults (if defaults have changed)
            CompilerToolsVector Commands[ctCount];
            CompilerSwitches    Switches;
            CompilerVarOptions  DeviceOptionsOpt;
            CompilerOptions     CompilerOptionsOpt;
            CompilerOptions     AssemblerOptionsOpt;
            CompilerOptions     LinkerOptionsOpt;
            RegExArray          RegExes;
        };
        MirrorSettings m_Mirror;
        bool           m_Mirrored; // flag to only mirror the settings once
};

#endif // COMPILER_H
