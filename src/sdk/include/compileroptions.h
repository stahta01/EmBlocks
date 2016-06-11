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

#ifndef COMPILEROPTIONS_H
#define COMPILEROPTIONS_H

#include <wx/dynarray.h>
#include <wx/intl.h>
#include <wx/string.h>
#include "settings.h"

#define FLAG_EXCL_OPT          0x0100
#define FLAG_CMB_LEADER_OPT    0x0200
#define FLAG_CMB_MEMBER_OPT    0x0400

#define MASK_GROUP_OPT         0x00FF

struct CompOption
{
	// following comments are an example of an option
	wxString name;			// "Profile code"
	wxString option;		// "-pg"
	bool enabled;			// true/false
	unsigned int group;     // For radio button grouping
	wxString category;		// "Profiling"
	bool doChecks;			// true/false
	wxString checkAgainst;	// "-O -O1 -O2 -O3 -Os" (will check for these options and if any of them is found, will display the following message)
	wxString checkMessage;	// "You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."
};

WX_DEFINE_ARRAY(CompOption*, OptionsArray);

class DLLIMPORT CompilerOptions
{
	public:
		CompilerOptions();
		CompilerOptions(const CompilerOptions& other);
		CompilerOptions& operator=(const CompilerOptions& other);
		~CompilerOptions();
		void ClearOptions();
		void AddOption(CompOption* coption);
		void AddOption(const wxString& name,
						const wxString& option,
						const wxString& category = _("General"),
						unsigned int optGroup = 0,
						bool doChecks = false,
						const wxString& checkAgainst = wxEmptyString,
						const wxString& checkMessage = wxEmptyString);
		unsigned int GetCount() const { return m_Options.GetCount(); }
		bool DeleteOptionByName(const wxString& name);
		CompOption* GetOption(int index){ return m_Options[index]; }
		CompOption* GetOptionByName(const wxString& name);
		CompOption* GetOptionByOption(const wxString& option);
	protected:
	private:
		OptionsArray m_Options;
};

#endif // COMPILEROPTIONS_H
