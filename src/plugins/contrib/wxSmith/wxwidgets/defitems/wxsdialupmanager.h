/** \file wxsdialupmanager.h
*
* This file is part of wxSmith plugin for Em::Blocks Studio
* Copyright (C) 2006-2010 Gary Harris
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef WXSDIALUPMANAGER_H
#define WXSDIALUPMANAGER_H

#include "../wxstool.h"

/** \brief Class for wxDialUpManager tool. */
class wxsDialUpManager: public wxsTool
{
    public:

        wxsDialUpManager(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual void OnEnumToolProperties(long Flags);

        bool			m_bAutoCheckOnlineStatus;		//!< Automatically check online status.
        long			m_iAutoCheckInterval;					//!< Automatic online status check interval (seconds).
#ifndef __WXMSW__
        wxString	m_sWellKnownHost;						//!< On Unix, the well known host used to check whether we're connected to the internet.
        long			m_iPortNo;										//!< On Unix, the well known host's port no.
        wxString	m_sDialCommand;						//!< On Unix, the dial command.
        wxString	m_sHangUpCommand;				//!< On Unix, the hang up command.
#endif
};

#endif