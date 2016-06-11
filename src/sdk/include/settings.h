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

	@version $Revision: 103 $:
    @author  $Author: gerard $:
    @date    $Date: 2014-02-04 08:27:07 +0100 (Tue, 04 Feb 2014) $:
*/

#ifndef SETTINGS_H
#define SETTINGS_H


#define CONFIGURE_LOC      "1.40"  // The default/global settings  appdata\emblocks\<xxx>
#define RELEASE            "1.44"
#define REVERSION          "1"
#define APP_NAME           "EmBlocks"
#define STANDARD_DATA_PATH "/share/emblocks"
#define KEY_NAME           "EmBlocks."
#define CONFIG_PATH


#define PROJECT_FILE_VERSION_MAJOR 1
#define PROJECT_FILE_VERSION_MINOR 0  //6

// this is the plugins SDK version number
// it will change when the SDK interface breaks
#define PLUGIN_SDK_VERSION_MAJOR   1
#define PLUGIN_SDK_VERSION_MINOR   0
#define PLUGIN_SDK_VERSION_RELEASE 0

#define DEFAULT_ARRAY_SEP_CHAR    ';'
#define CONFIG_VERSION             1


#define RSS_UPDATE_TIME 300000L   // 5 minutes RSS update timer
#define RSS_URL  "http://www.emblocks.org/rss/app_rss.php"

/*
    { Extension (wxString), Description (wxString), IconIndex (int) }
      Note: "index" is the index of the icon resource in "resources.rc"
            Keep all indices in sync with icon indices in "resources.rc"!
*/

#define NUMBER_OF_CORE_ASSOC 2 // The number of core associations running from to start

#define PROGRAM_FILE_ASSOCIATIONS()  \
    { FileFilters::EMBLOCKS_EXT,   _T("project file"),                  1 },\
    { FileFilters::WORKSPACE_EXT,  _T("workspace file"),                2 },\
    { FileFilters::C_EXT,          _T("C source file"),                 3 },\
    { FileFilters::CC_EXT,         _T("C++ source file"),               4 },\
    { FileFilters::CPP_EXT,        _T("C++ source file"),               4 },\
    { FileFilters::H_EXT,          _T("Header file"),                   5 }
/*
    { _T("cg"),                    _T("cg source file"),                8 },\
    { FileFilters::D_EXT,          _T("D source file"),                 9 },\
    { FileFilters::ASM_EXT,        _T("ASM source file"),               3 },\
    { FileFilters::S_EXT,          _T("ASM source file"),               3 }
*/

/*
Exclude VC++, because it has silly constraints on importing/exporting classes
from DLLs. Instead, we build "sdk" as a static library
*/
#if defined(__WXMSW__)
	#ifndef DLLIMPORT
		#if EXPORT_LIB
			#define DLLIMPORT __declspec (dllexport)
		#else
			#define DLLIMPORT __declspec (dllimport)
		#endif // EXPORT_LIB
	#endif // DLLIMPORT
	#ifndef EVTIMPORT
		#if EXPORT_EVENTS
			#define EVTIMPORT __declspec (dllexport)
		#else
			#define EVTIMPORT __declspec (dllimport)
		#endif // EXPORT_EVENTS
	#endif // EVTIMPORT
#else
	#define DLLIMPORT
	#define EVTIMPORT
#endif

#endif // SETTINGS_H
