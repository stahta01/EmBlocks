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

#ifndef PERSONALITYMANAGER_H
#define PERSONALITYMANAGER_H

#include "settings.h"
#include "manager.h"
#include <wx/dynarray.h>

/** @brief Manage different personalities.
  *
  * Personalities are different profiles of appearence for Em::Blocks.
  * For instance, the default personality is the full IDE. You could create
  * a "Text editor" personality that is light-weight, tailored to "simple" text editing.
  * In this example "Text editor" personality, the manager windows would be hidden
  * and no plugins would be loaded, making Em::Blocks perfect for text editing
  * (with all the powerful features of its embedded editor).
  *
  * The user can create as many personalities as needed. The work-in-progress
  * file associations manager (win32 only) will work with personalities so that
  * different file types launch Em::Blocks using a different personality...
  *
  * To select the desired personality when launching Em::Blocks, the user can
  * use the command-line switch "--personality". If "--personality=ask" is passed
  * in the command line, a selection box will be displayed for the user to choose
  * the desired personality.
  */
class DLLIMPORT PersonalityManager : public Mgr<PersonalityManager>
{
    static wxString pers;

    PersonalityManager();

	public:
        friend class Mgr<PersonalityManager>;
        friend class Manager; // give Manager access to our private members

        /// Use this once, on program startup to set the working personality
        void SetPersonality(const wxString& personality, bool createIfNotExist = false);
        /// Get the working personality string
        const wxString GetPersonality();
        /// Get a list of all the known personalities
        const wxArrayString GetPersonalitiesList();
};

#endif // PERSONALITYMANAGER_H
