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
#ifndef PROJECTBUILDTARGET_H
#define PROJECTBUILDTARGET_H

#include "settings.h"
#include "globals.h"
#include "compiletargetbase.h"
#include "projectfile.h"
#include <wx/dynarray.h>
#include <wx/filename.h>
#include <wx/list.h>
#include <wx/treectrl.h>

class cbProject;
class ProjectBuildTarget;

WX_DEFINE_ARRAY(ProjectBuildTarget*, BuildTargets);

/** Represents a Em::Blocks project build target. */
class DLLIMPORT ProjectBuildTarget : public CompileTargetBase
{
	public:
		/// Constructor
		ProjectBuildTarget(cbProject* parentProject);
		/// Destructor
		~ProjectBuildTarget();

        /** @return The target's parent project. */
        virtual cbProject* GetParentProject();
        /** @return The full title, i.e. "projectname - targetname". */
        virtual wxString GetFullTitle() const;

        //properties

        /** @return A string containing a list of all the external files this target depends on.
          * If any of the files in this list is newer than the target's output, the target is relinked.
          */
        virtual const wxString& GetExternalDeps() const;

        /** Set a list of all the external files this targets depends on.
          * If any of the files in this list is newer than the target's output, the target is relinked.
          * @param deps A string containing the list of files.
          */
        virtual void SetExternalDeps(const wxString& deps);

        /** @return A string containing a list of additional output files, besides the main output.
          * If any of the files in this list is older than the list returned by
          * GetExternalDeps(), the target is relinked.
          */
        virtual const wxString& GetAdditionalOutputFiles() const;

        /** Set a list of all additional output files this targets creates, besides its main output.
          * If any of the files in this list is older than the list returned by
          * GetExternalDeps(), the target is relinked.
          * @param files A string containing the list of additional files.
          */
        virtual void SetAdditionalOutputFiles(const wxString& files);


        virtual void SetTargetType(const TargetType& pt); // overriden

        /** Targets to be compiled (if necessary) before this one.
          * Add a target to the list of dependencies of this target. Be careful
          * not to add a target more than once.
          * @param target The build target to add as a dependency.
          */
        virtual void AddTargetDep(ProjectBuildTarget* target);

        /** @return A list of dependency targets of this target. */
        virtual BuildTargets& GetTargetDeps();

        /** Provides an easy way to iterate all the files belonging in this target.
          * @return A list of files belonging in this target. */
        virtual FilesList& GetFilesList(){ return m_Files; };

        /** Set the create hex file for this Target with external tool.
          * @param 'true' to create hex file.
          */
        virtual void SetCreateHex(bool b) {m_CreateHex = b;  };

         /** @return If a hex file must be created for this target with external tool.
         */
        virtual bool GetCreateHex() {return m_CreateHex;}

    private:
        friend class ProjectFile; // to allow it to add/remove files in FilesList
        cbProject* m_Project;
        wxString m_ExternalDeps;
        wxString m_AdditionalOutputFiles;
        BuildTargets m_TargetDeps;
        FilesList m_Files;
        bool m_CreateHex;
};

#endif // PROJECTBUILDTARGET_H
