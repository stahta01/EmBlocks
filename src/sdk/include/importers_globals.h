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

#ifndef IMPORTERS_GLOBALS_H
#define IMPORTERS_GLOBALS_H

#include "settings.h"

/**
  * This namespace holds some global flags for project/workspace importers
  * to inter-communicate.\n
  * Basically, workspace importers set/unset these flags so that project
  * importers know how to behave.
  *
  * To explain this a bit further, imagine a MS Visual Studio solution (workspace)
  * file containing 50 projects (yes, I have in mind one!).
  * Without this class, for each project being imported, the user would be asked
  * which compiler to use (one dialog) and which targets to import (another dialog).
  * Imagine the user's frustration having to answer to 100 dialogs (50 x 2)!
  *
  * Using these flags, the workspace importer can notify the project importer
  * what to do, without asking the user each and every import.
  *
  * After the workspace importer is done, it should call the ResetDefaults() function
  * to revert everything to defaults.
  */
namespace ImportersGlobals
{
    // reset all flags to defaults
    extern DLLIMPORT void ResetDefaults();
    // ask which compiler to use for the imported project?
    extern DLLIMPORT bool UseDefaultCompiler;
    // ask which targets to import?
    extern DLLIMPORT bool ImportAllTargets;
};

#endif // IMPORTERS_GLOBALS_H
