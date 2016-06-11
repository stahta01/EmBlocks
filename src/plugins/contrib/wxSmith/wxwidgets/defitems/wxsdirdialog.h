/** \file wxsdirdialog.h
*
* This file is part of wxSmith plugin for Em::Blocks Studio
* Copyright (C) 2007  Bartlomiej Swiecki
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
* $Revision: 4 $
* $Id: wxsdirdialog.h 4 2013-11-02 15:53:52Z gerard $
* $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsdirdialog.h $
*/

#ifndef WXSDIRDIALOG_H
#define WXSDIRDIALOG_H

#include "../wxstool.h"

class wxsDirDialog: public wxsTool
{
    public:

        wxsDirDialog(wxsItemResData* Data);

    private:

        virtual void OnBuildCreatingCode();
        virtual void OnEnumToolProperties(long Flags);

        wxString m_Message;
        wxString m_DefaultPath;
};

#endif