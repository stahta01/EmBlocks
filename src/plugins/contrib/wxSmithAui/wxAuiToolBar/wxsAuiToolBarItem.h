/*
* This file is part of wxSmithAui plugin for Em::Blocks Studio
* Copyright (C) 2008-2009  César Fernández Domínguez
*
* wxSmithAui is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmithAui is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmithAui. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WXSAUITOOLBARITEM_H
#define WXSAUITOOLBARITEM_H

#include "wxsAuiToolBarItemBase.h"

/** \brief Class for wxsAuiToolBarItem widget */
class wxsAuiToolBarItem: public wxsAuiToolBarItemBase
{
    public:

        wxsBitmapData m_Bitmap;
        wxsBitmapData m_DisabledBitmap;
        wxString      m_ShortHelp;
        wxString      m_LongHelp;
        wxItemKind    m_ItemKind;
        bool          m_DropDown;

        wxsAuiToolBarItem(wxsItemResData* Data);

    private:

        virtual void OnEnumItemProperties(long Flags);
};

#endif