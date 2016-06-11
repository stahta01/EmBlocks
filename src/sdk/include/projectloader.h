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

#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <wx/hashmap.h>
#include "ibaseloader.h"


class cbProject;
class ProjectBuildTarget;
class ProjectFile;

WX_DECLARE_STRING_HASH_MAP(wxString, CompilerSubstitutes);

/** Em::Blocks project file loader. */
class DLLIMPORT ProjectLoader : public IBaseLoader
{
    public:
        /** Constructor.
          * @param project The project to handle (load/save). */
        ProjectLoader(cbProject* project);
        /// Destructor.
        virtual ~ProjectLoader();

        /** Open a file.
          * @param filename The file to open.
          * @return True on success, false on failure. */
        bool Open(const wxString& filename);

        /** Save a file.
          * @param filename The file to save.
          * @return True on success, false on failure. */
        bool Save(const wxString& filename);

        /** Open a file.
          * This version of Open, will return a copy of the \<Extensions\> element (if found).
          * @param filename The file to open.
          * @param ppExtensions A pointer to a pointer of type TiXmlElement. This is where
          * the copy of the \<Extensions\> element will be placed.
          * @return True on success, false on failure. */
        bool Open(const wxString& filename, TiXmlElement** ppExtensions);

        /** Save a file.
          * This version of Save, can override the \<Extensions\> element.
          * @param filename The file to save.
          * @param pExtensions A pointer of type TiXmlElement. This will be added as
          * the \<Extensions\> element.
          * @return True on success, false on failure. */
        bool Save(const wxString& filename, TiXmlElement* pExtensions);

        /** Export a target as a new project.
          * In other words, save a copy of the project containing only the specified target.
          * @param filename The new project filename.
          * @param pExtensions A pointer of type TiXmlElement. This will be added as
          * the \<Extensions\> element.
          * @param onlyTarget The target name. If empty, it's like saving the project under a different name
          * (i.e. all targets are exported to the new project). */
        bool ExportTargetAsProject(const wxString& filename, const wxString& onlyTarget, TiXmlElement* pExtensions);

        /** @return True if the file was upgraded after load, false if not. */
        bool FileUpgraded(){ return m_Upgraded; }

        /** @return True if the file was modified while loading, false if not. This is usually true if FileUpgraded() returned true. */
        bool FileModified(){ return m_OpenDirty; }
    protected:
        void DoProjectOptions(TiXmlElement* parentNode);
        void DoDeviceOptions(TiXmlElement* parentNode, ProjectBuildTarget* target = 0L);
        void DoCompilerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target = 0L);
        void DoAssemblerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target = 0L);
        void DoLinkerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target = 0L);
        void DoIncludesOptions(TiXmlElement* parentNode, ProjectBuildTarget* target = 0L);
        void DoLibsOptions(TiXmlElement* parentNode, ProjectBuildTarget* target = 0L);
        void DoExtraCommands(TiXmlElement* parentNode, ProjectBuildTarget* target = 0L);
        void DoMakeCommands(TiXmlElement* parentNode, CompileTargetBase* target);
        void DoVirtualTargets(TiXmlElement* parentNode);

        void DoBuild(TiXmlElement* parentNode);
        void DoBuildTarget(TiXmlElement* parentNode);
        void DoBuildTargetOptions(TiXmlElement* parentNode, ProjectBuildTarget* target);

        void DoEnvironment(TiXmlElement* parentNode, CompileOptionsBase* base);

        void DoUnits(TiXmlElement* parentNode);
        void DoUnitOptions(TiXmlElement* parentNode, ProjectFile* file);
    private:
        // convenience functions, used in Save()
        TiXmlElement* AddElement(TiXmlElement* parent, const char* name, const char* attr = 0, const wxString& attribute = wxEmptyString);
        TiXmlElement* AddElement(TiXmlElement* parent, const char* name, const char* attr, int attribute);
        void AddArrayOfElements(TiXmlElement* parent, const char* name, const char* attr, const wxArrayString& array);
        void SaveEnvironment(TiXmlElement* parent, CompileOptionsBase* base);

        // accepts a questionable compiler index and returns a valid compiler index
        // (popping up a selection dialog if needed)
        wxString GetValidCompilerID(const wxString& proposal, const wxString& scope);

        ProjectLoader(){} // no default ctor

        cbProject* m_pProject;
        bool m_Upgraded;
        bool m_OpenDirty; // set this to true if the project is loaded but modified (like the case when setting another compiler, if invalid)
        CompilerSubstitutes m_CompilerSubstitutes;
};

#endif // PROJECTLOADER_H
