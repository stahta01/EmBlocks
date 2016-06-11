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
#include <map>
#include <wx/arrstr.h>
#include "filefilters.h"
#include "globals.h"

typedef std::map<wxString, wxString> FileFiltersMap;
static FileFiltersMap s_Filters;

static size_t s_LastFilterAllIndex = 0;

// Let's add some default extensions.
// The rest will be added by editor lexers ;)
void FileFilters::AddDefaultFileFilters()
{
    if (s_Filters.size() != 0)
        return;
    Add(EM_WORKSPACE_OPEN_FILTER_TXT, _T("*.eworkspace"));
    Add(EM_PROJECT_OPEN_FILTER_TXT,   _T("*.ebp"));
//    Add(EM_STANDARD_OPEN_FILTER_TXT,  _T("*.ebp;*.eworkspace"));
   // Add(_T("Mplab8 project files"),   _T("*.mcp"));
}

bool FileFilters::Add(const wxString& name, const wxString& mask)
{
    if (name.IsEmpty() || mask.IsEmpty())
        return false; // both must be valid
    if (mask.Index(_T(',')) != wxString::npos)
    {
        // replace commas with semicolons
        wxString tmp = mask;
        while (tmp.Replace(_T(","), _T(";")))
            ;
        s_Filters[name] = tmp;
    }
    else
        s_Filters[name] = mask;
    return true;
}


void FileFilters::FilterNameRemove(const wxString& FilterName)
{
    if (FilterName.IsEmpty())
        return; // both must be valid

    s_Filters.erase(FilterName);
}

wxString FileFilters::GetFilterString(const wxString& ext)
{
    size_t count = 0;
    wxString ret;
    for (FileFiltersMap::iterator it = s_Filters.begin(); it != s_Filters.end(); ++it)
    {
        if (!ext.IsEmpty())
        {
            // filter based on parameter
            bool match = false;
            wxArrayString array = GetArrayFromString(it->second, _T(";"), true);
            for (size_t i = 0; i < array.GetCount(); ++i)
            {
                if (ext.Matches(array[i]))
                {
                    match = true;
                    break;
                }
            }
            if (!match)
                continue; // filtered
        }
        ++count;
        if (!ret.IsEmpty())
            ret << _T('|');
        ret << it->first << _T('|') << it->second;
    }

    // last filter is always "All"
    if (!ret.IsEmpty())
        ret << _T('|');
    ret << GetFilterAll();

    s_LastFilterAllIndex = count;

    return ret;
}

wxString FileFilters::GetFilterAll()
{
    s_LastFilterAllIndex = 0;
    if(platform::windows)
        return _("All files (*.*)|*.*");
    else
        return _("All files (*)|*");
}

size_t FileFilters::GetIndexForFilterAll()
{
    return s_LastFilterAllIndex;
}

bool FileFilters::GetFilterIndexFromName(const wxString& FiltersList, const wxString& FilterName, int& Index)
{
    bool bFound = false;
    // the List will contain 2 entries per type (description, mask)
    wxArrayString List = GetArrayFromString(FiltersList, _T("|"), true);
    int LoopEnd = static_cast<int>(List.GetCount());
    for(int idxList = 0; idxList < LoopEnd; idxList+=2)
    {
        if(List[idxList] == FilterName)
        {
            Index = idxList/2;
            bFound = true;
            break;
        }
    } // end for : idx : idxList
    return bFound;
} // end of GetFilterIndexFromName

bool FileFilters::GetFilterNameFromIndex(const wxString& FiltersList, int Index, wxString& FilterName)
{    // we return the name (not the mask)
    bool bFound = false;
    // the List will contain 2 entries per type (description, mask)
    wxArrayString List = GetArrayFromString(FiltersList, _T("|"), true);
    int LoopEnd = static_cast<int>(List.GetCount());
    if(2*Index < LoopEnd)
    {
        FilterName = List[2*Index];
        bFound = true;
    }
    return bFound;
} // end of GetFilterStringFromIndex

// define some constants used throughout E::B
const wxString FileFilters::WORKSPACE_EXT       = _T("eworkspace");
const wxString FileFilters::EMBLOCKS_EXT        = _T("ebp");
//const wxString FileFilters::ASM_EXT             = _T("asm");
const wxString FileFilters::D_EXT               = _T("d");


const wxString FileFilters::H_EXT               = _T("h");
//const wxString FileFilters::S_EXT               = _T("s");
const wxString FileFilters::OBJECT_EXT          = _T("o");
const wxString FileFilters::MAP_EXT             = _T("map");
const wxString FileFilters::HEX_EXT             = _T("hex");
const wxString FileFilters::LIBRARY_EXT         = _T("a");
const wxString FileFilters::EXECUTABLE_EXT      = _T("elf");
const wxString FileFilters::XML_EXT             = _T("xml");
const wxString FileFilters::SCRIPT_EXT          = _T("script");
const wxString FileFilters::MPLAB8PRJ_EXT       = _T("mcp");
const wxString FileFilters::ELAY_EXT            = _T("elay");

// Only for file association
const wxString FileFilters::C_EXT               = _T("c");
const wxString FileFilters::CC_EXT              = _T("cc");
const wxString FileFilters::CPP_EXT             = _T("cpp");


// dot.ext version

    // a dot *and* the extension, e.g. ".exe"
const wxString FileFilters::WORKSPACE_DOT_EXT       = _T(".") + WORKSPACE_EXT;
const wxString FileFilters::EMBLOCKS_DOT_EXT        = _T(".") + EMBLOCKS_EXT;
//const wxString FileFilters::ASM_DOT_EXT             = _T(".") + ASM_EXT;
const wxString FileFilters::D_DOT_EXT               = _T(".") + D_EXT;
//const wxString FileFilters::C_DOT_EXT               = _T(".") + C_EXT;
//const wxString FileFilters::CC_DOT_EXT              = _T(".") + CC_EXT;
//const wxString FileFilters::CPP_DOT_EXT             = _T(".") + CPP_EXT;
const wxString FileFilters::H_DOT_EXT               = _T(".") + H_EXT;
//const wxString FileFilters::S_DOT_EXT               = _T(".") + S_EXT;
const wxString FileFilters::OBJECT_DOT_EXT          = _T(".") + OBJECT_EXT;
const wxString FileFilters::MAP_DOT_EXT             = _T(".") + MAP_EXT;
const wxString FileFilters::LIBRARY_DOT_EXT         = _T(".") + LIBRARY_EXT;
const wxString FileFilters::EXECUTABLE_DOT_EXT      = _T(".") + EXECUTABLE_EXT;
const wxString FileFilters::XML_DOT_EXT             = _T(".") + XML_EXT;
const wxString FileFilters::SCRIPT_DOT_EXT          = _T(".") + SCRIPT_EXT;
const wxString FileFilters::MPLAB8PRJ_DOT_EXT       = _T(".") + MPLAB8PRJ_EXT;
const wxString FileFilters::ELAY_DOT_EXT            = _T(".") + ELAY_EXT;
