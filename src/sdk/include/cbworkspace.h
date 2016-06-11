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

#ifndef CBWORKSPACE_H
#define CBWORKSPACE_H

#include <wx/string.h>
#include <wx/filename.h>

#include "globals.h"

/**
  * @brief A workspace class.
  *
  * Workspaces are just collections of projects. By loading a workspace,
  * all the projects it contains are loaded.\n
  * There is always a workspace open. The default one does not contain any projects.\n
  * Currently, no inter-project dependencies are supported but at some point
  * they will be ;)
  *
  * @note The way WorkspaceLoader works now, requires that you save and delete the
  * loaded workspace *before* creating a new one...
  */
class cbWorkspace
{
    public:
        /** @brief Constructor
          *
          * @param filename The file from which to load the workspace. If this
          *        parameter is empty, the default workspace is loaded.
          */
        cbWorkspace(const wxString& filename = DEFAULT_WORKSPACE);

        /** @brief Destructor */
        virtual ~cbWorkspace();

        /** @brief Save the workspace
          *
          * @param force If false (the default), the workspace will not be written to disk,
          * if it is not marked as modified.
          * @return True if saving succeeded, false if not.
          */
        virtual bool Save(bool force = false);

        /** @brief Save the workspace under a different filename
          *
          * @param filename The name of the file to save. If empty string, a file dialog will be shown.
          * @return True if saving succeeded, false if not.
          * @note If the filename parameter is empty, a file dialog to choose
          * the filename to save will be displayed.
          */
        virtual bool SaveAs(const wxString& filename);

        /** @brief Get the workspace file's name
          *
          * @return The name of the file this workspace was loaded from.
          */
        virtual wxString GetFilename() const
        {
          return m_Filename.GetFullPath();
        }

        /** @brief Get the workspace's title
          *
          * @return The title of the workspace.
          */
        virtual wxString GetTitle() const
        {
          return m_Title;
        }

        /** @brief Set the workspace's title
          *
          * @param title The new title.
          */
        virtual void SetTitle(const wxString& title);

        /** @brief Was this workspace loaded successfully?
          *
          * @return True if the workspace was loaded successfully, false if not.
          * @note Because the only way to load a workspace is through its
          * constructor, and because we don't use exceptions, this is the only
          * way to know if loading succeeded.
          */
        virtual bool IsOK() const { return m_IsOK; }

        /** @brief Is this workspace the Em::Blocks default?
          *
          * @return True if the workspace is the default, false if not.
          */
        virtual bool IsDefault() const { return m_IsDefault; }

        /** @brief Is this workspace modified?
          *
          * @return True if the workspace is modified, false if not.
          * @note A workspace is considered modified when projects
          * are added-to/removed-from it, when the project's order
          * is changed or when the active project is changed.
          */
        virtual bool GetModified() const { return m_Modified; }

        /** @brief Mark the workspace as modified or not
          *
          * @param modified If true, the workspace will be marked as modified. If
          * false, the workspace will be marked as unmodified.
          */
        virtual void SetModified(bool modified);
    private:
        bool m_IsOK; // succeeded loading?
        bool m_IsDefault; // is this the Em::Blocks default workspace?
        bool m_Modified; // is it modified?
        wxFileName m_Filename; // filename
        wxString m_Title; // title

        void Load(); // utility function
};

#endif // CBWORKSPACE_H
