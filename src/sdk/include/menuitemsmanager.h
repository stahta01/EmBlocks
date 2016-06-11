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

#ifndef MENUITEMSMANAGER_H
#define MENUITEMSMANAGER_H

#include "settings.h"
#include <wx/menu.h>

WX_DEFINE_ARRAY(wxMenuItem*, MenuItemsList);

/**
  * @brief Manager for wxMenuItem pointers.
  *
  * This class manages an array of wxMenuItem pointers. Usually used by
  * classes that need to create menu items in the app and, at some point,
  * remove them *without* messing with other menu items, created by other
  * classes. Useful for plugins.\n
  * To use it, add a MenuItemsManager variable in your class and then
  * use MenuItemsManager::Add() to add menu items to a menu (instead of
  * wxMenu::Append). When you no longer want those menu items, call
  * MenuItemsManager::Clear(). That's it.
  *
  * @author Yiannis Mandravellos
  */
class DLLIMPORT MenuItemsManager
{
    public:
        MenuItemsManager(bool autoClearOnDestroy = true);
        virtual ~MenuItemsManager();

        virtual wxMenuItem* CreateFromString(const wxString& menuPath, int id);

        virtual wxMenuItem* Add(wxMenu* parent, int id, const wxString& caption, const wxString& helptext);
        virtual wxMenuItem* Insert(wxMenu* parent, int index, int id, const wxString& caption, const wxString& helptext);
        virtual void Clear();
    protected:
        MenuItemsList m_MenuItems; // The managed array of wxMenuItem pointers
        bool m_AutoClearOnDestroy; // if true, the menus are cleared in the destructor
    private:
};

#endif // MENUITEMSMANAGER_H
