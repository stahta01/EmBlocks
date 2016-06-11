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

#ifndef COMPILETARGETBASE_H
#define COMPILETARGETBASE_H

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#endif /* ATTRIBUTE_UNUSED */

#include "compileoptionsbase.h"

/** Enum that defines the option's relation types */
enum OptionsRelationType
{
    ortDeviceOptions = 0, /**< Device option */
    ortCompilerOptions, /**< Compiler option */
    ortAssemblerOptions, /**< Assembler option */
    ortLinkerOptions, /**< Linker option */
    ortIncludeDirs, /**< Compiler include dir option */
    ortLibDirs, /**< Linker include dir option */
    ortLast
};

/** Option's relation */
enum OptionsRelation
{
    orUseParentOptionsOnly = 0, /**< The option uses parent options only */
    orUseTargetOptionsOnly, /**< The option uses target options only */
    orPrependToParentOptions, /**< The option uses parent options appended to target options */
    orAppendToParentOptions /**< The option uses target options appended to parent options */
};

/** Enum to define the type of output the target produces */
enum TargetType
{
    ttExecutable    = 0, /**< Target produces an executable */
    ttLibrary       = 1  /**< Target produces an archive library */
};

enum MakeCommand
{
    mcClean = 0,
    mcDistClean,
    mcBuild,
    mcCompileFile,
    mcAskRebuildNeeded,
    mcSilentBuild,

    /// *Don't* use this. It's only used internally for enumerations...
    mcLast
};


/**
  * @brief Base class for build target classes
  * Each Em::Blocks project
  * consists of at least one target. Each target has different settings,
  * e.g.:
  * \li Build options,
  * \li Output type,
  * \li Execution parameters, etc.
  * \n\n
  * This class holds the settings of one build target.
 */
class DLLIMPORT CompileTargetBase : public CompileOptionsBase
{
    public:
        CompileTargetBase();
        virtual ~CompileTargetBase();

        virtual const wxString& GetFilename() const;
        virtual const wxString& GetTitle() const; ///< Read the target's title
        virtual void SetTitle(const wxString& title); ///< Set the target's title
        virtual void SetOutputFilename(const wxString& filename); ///< Set the target's output filename
        virtual void SetObjectOutput(const wxString& dirname); ///< Set the target's objects output dir
        virtual void SetDepsOutput(const wxString& dirname); ///< Set the target's dependencies output dir
        virtual OptionsRelation GetOptionRelation(OptionsRelationType type) const; ///< Read the target's options relation for \c type
        virtual void SetOptionRelation(OptionsRelationType type, OptionsRelation rel); ///< Set the target's options relation for \c type to \c rel
        virtual wxString GetObjectOutput() const; ///< Read the target's objects output dir
        virtual wxString GetDepsOutput() const; ///< Read the target's dependencies output dir
        virtual wxString GetOutputFilename(); ///< Read the target's output filename
        virtual wxString SuggestOutputFilename(); ///< Suggest a filename based on the target's type
        virtual wxString GetExecutableFilename() const; ///< Read the target's executable filename (produced if target type is ttExecutable)
        virtual wxString GetLibraryFilename(); ///< Read the target's library filename (produced if target type is ttLibrary)
        virtual wxString GetBasePath() const; ///< Read the target's base path, e.g. if GetFilename() returns "/usr/local/bin/xxx", base path will return "/usr/local/bin"
        virtual void SetTargetType(TargetType pt); ///< Set the target's type to \c pt
        virtual TargetType GetTargetType() const; ///< Read the target's type
        virtual void SetCompilerID(const wxString& id); ///< Set the target's compiler
        virtual const wxString& GetCompilerID() const { return m_CompilerId; } ///< Read the target's compiler
        virtual wxString GetMakeCommandFor(MakeCommand cmd) const { return m_MakeCommands[cmd]; } ///< Get the "make" command used for @c cmd
        virtual void SetMakeCommandFor(MakeCommand cmd, const wxString& make); ///< Set the "make" command used for @c cmd
        virtual bool MakeCommandsModified() const { return m_MakeCommandsModified; } ///< True if any of the "make" commands is modified.


        /** Just to make plugins backward compatible with C::B
          * We don't use working directories on targets (that is an external device).
          */
        virtual void SetWorkingDir(const wxString& dirname ATTRIBUTE_UNUSED) {};

        /** Just to make plugins backward compatible with C::B
          * We don't use working directories on targets (that is an external device) so we will
          * return our directory were the output file will be stored
          */
        virtual wxString GetWorkingDir() { wxFileName fname(GetOutputFilename()); return fname.GetFullPath(); };

    protected:
        friend class cbProject;

        wxString m_Filename;
        wxString m_Title;
        mutable wxString m_OutputFilename;
        wxString m_ObjectOutput;
        wxString m_DepsOutput;
        OptionsRelation m_OptionsRelation[ortLast];
        TargetType m_TargetType;
        wxString m_CompilerId;
        wxString m_MakeCommands[mcLast];
        bool m_MakeCommandsModified;
    private:
        void GenerateTargetFilename(wxString& filename) const;
};

#endif // COMPILETARGETBASE_H
