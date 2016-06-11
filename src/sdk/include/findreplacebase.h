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

#ifndef FINDREPLACEBASE_H
#define FINDREPLACEBASE_H

#include "scrollingdialog.h"
#include <wx/string.h>

class wxWindow;

class FindReplaceBase : public wxScrollingDialog
{
	public:
		FindReplaceBase(wxWindow* /*parent*/, const wxString& /*initial*/, bool /*hasSelection*/){}
		virtual ~FindReplaceBase(){}
		virtual wxString GetFindString() const = 0;
		virtual wxString GetReplaceString() const = 0;
		virtual bool IsFindInFiles() const = 0;
		virtual bool GetDeleteOldSearches() const = 0;
		virtual bool GetMatchWord() const = 0;
		virtual bool GetStartWord() const = 0;
		virtual bool GetStartFile() const = 0;
		virtual bool GetMatchCase() const = 0;
		virtual bool GetFixEOLs() const = 0;
		virtual bool GetRegEx() const = 0;
		virtual bool GetAutoWrapSearch() const = 0;
		virtual bool GetFindUsesSelectedText() const = 0;
		virtual int GetDirection() const = 0;
		virtual int GetOrigin() const = 0;
		virtual int GetScope() const = 0;
		virtual bool GetRecursive() const = 0; // for find in search path
		virtual bool GetHidden() const = 0; // for find in search path
		virtual wxString GetSearchPath() const = 0; // for find in search path
		virtual wxString GetSearchMask() const = 0; // for find in search path
};

#endif // FINDREPLACEBASE_H
