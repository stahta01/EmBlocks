/*
 * This file is part of the code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
*/
/*
    Copyright (C) Em::Blocks 2011-2013

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


    @version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:

 */

#include <sdk.h>
#ifndef CB_PRECOMP
    #include <wx/utils.h>
    #include <wx/intl.h>
    #include "cbplugin.h"
    #include "configmanager.h"
#endif
#include "appglobals.h"

namespace appglobals
{
    const wxString AppVendor              = _T("Em::Blocks");
    const wxString AppName                = _T("Em::Blocks");
    const wxString AppVersion             = _T(RELEASE);
    const wxString AppActualVersionVerb   = _T("Release " RELEASE "  rev " REVERSION) ;
    const wxString AppActualVersion       = _T(RELEASE "-r") + ConfigManager::GetRevisionString();

    const wxString AppSDKVersion = wxString::Format(wxT("%d.%d.%d"),
                                                    static_cast<int>(PLUGIN_SDK_VERSION_MAJOR),
                                                    static_cast<int>(PLUGIN_SDK_VERSION_MINOR),
                                                    static_cast<int>(PLUGIN_SDK_VERSION_RELEASE));

    const wxString AppUrl                   = _T("http://www.emblocks.org");
    const wxString AppContactEmail          = _T("info@emblocks.org");

    #if defined(__WXMSW__)
      const wxString AppPlatform = _T("Windows");
    #elif defined(__WXOS2__)
      const wxString AppPlatform = _T("OS/2");
    #elif defined(__WXMAC__) || defined(__WXCOCOA__)
      const wxString AppPlatform = _T("Mac OS X");
    #elif defined(__APPLE__)
      const wxString AppPlatform = _T("Darwin");
    #elif defined(__FreeBSD__)
      const wxString AppPlatform = _T("FreeBSD");
    #elif defined(__UNIX__)
      const wxString AppPlatform = _T("Linux");
    #else
      const wxString AppPlatform = _T("Unknown");
    #endif

    const wxString AppWXAnsiUnicode = platform::unicode ? wxT("unicode") : wxT("ANSI");

    #if defined(_LP64) || defined(_WIN64)
    const wxString bit_type = wxT(" - 64 bit");
    #else
    const wxString bit_type = wxT(" - 32 bit");
    #endif

 //   const wxString AppBuildTimestamp = (wxString(wxT(__DATE__)) + wxT(", ") + wxT(__TIME__) + wxT(" - wx") + wxString(wxT(wxVERSION_NUM_DOT_STRING)) + wxT(" (") + AppPlatform + wxT(", ") + AppWXAnsiUnicode + wxT(")") + bit_type );
    const wxString AppBuildTimestamp = (wxString(wxT(__DATE__)) + wxT(", ") + wxT(__TIME__) + bit_type );

    const wxString DefaultBatchBuildArgs = _T("-na -nd --batch-build-notify");
};


