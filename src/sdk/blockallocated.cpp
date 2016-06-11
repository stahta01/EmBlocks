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
#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/log.h> // for wxSafeShowMessage()
    #include <wx/regex.h>
#endif
#include "blockallocated.h"

namespace BlkAllc
{

void DebugLog(wxString cn, int blockSize, int poolSize, int max_refs, int total_refs, int ref_count)
        {
            wxString s;
            wxString cn2;

            if(total_refs == 0)
                return; // pointless

            wxRegEx r(_T("^[A-Z]?[0-9]+(.*)"));
            if(r.Matches(cn))
                cn2 = r.GetMatch(cn, 1);

            s.Printf(_T("%s\n\n%d reserved pools of size %d (%d total objects)\n"
            "Maximum number of allocated objects: %d\n"
            "Total number of allocations: %d\n"
            "Number of stale objects: %d %s"),
            cn2.c_str(),
            blockSize, poolSize, blockSize * poolSize,
            max_refs, total_refs, ref_count, (ref_count == 0 ? _T("") : _T("(memory leak)")));

            wxSafeShowMessage(_T("Block Allocator"), s);
		} // end of DebugLog
};
