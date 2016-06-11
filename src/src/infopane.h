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

#ifndef INFOPANE_H
#define INFOPANE_H

#include <limits>
#include <logmanager.h>

#include "cbauibook.h"

static const int infopane_flags = wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_SPLIT;

class wxWindow;
class wxCommandEvent;
class wxMouseEvent;

class InfoPane : public cbAuiNotebook
{
    DECLARE_EVENT_TABLE()

    struct Page
    {
        Page() : icon(0), window(0), logger(0), indexInNB(std::numeric_limits<int>::min()), eventID(0), islogger(0) {};
        wxString title;
        wxBitmap* icon;
        wxWindow* window;
        Logger* logger;
        int indexInNB; // used to be "visible" flag: invisible is <0, any other value means visible
        int eventID;
        bool islogger;
    };

    WX_DEFINE_ARRAY(Page *, wxArrayOfPage);

    typedef int (*CompareFunction)(Page**, Page**);
    static int CompareIndexes(Page **p1, Page **p2);
    void ReorderTabs(CompareFunction cmp_f);
    wxBitmap defaultBitmap;


    wxArrayOfPage page;

    void OnMenu(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void ContextMenu(wxContextMenuEvent& event);
    void OnTabContextMenu(wxAuiNotebookEvent& event);
    void OnCloseClicked(wxAuiNotebookEvent& event);
    void DoShowContextMenu();
    int AddPagePrivate(wxWindow* p, const wxString& title, wxBitmap* icon = 0);
    bool InsertPagePrivate(wxWindow* p, const wxString& title, wxBitmap* icon = 0 , int index = -1);
public:

    InfoPane(wxWindow* parent);
    ~InfoPane();

    void Toggle(size_t index);
    void Hide(size_t i);

    /*
    *  Show a tab, or bring it to the foreground if already shown.
    *  Only use this method if crucial information is shown which the user must not miss, or if
    *  information is shown that requires input from the user (for example search results).
    *  In all other cases, it is the user's decision which tab he wants to look at, not yours.
    *  Note that you cannot hide a tab programatically.
    */
    void Show(size_t index);
    void Show(Logger* logger);
    void Hide(Logger* logger);
    void ShowNonLogger(wxWindow* p);
    void HideNonLogger(wxWindow* p);

    int GetPageIndexByWindow(wxWindow* win);
    int GetCurrentPage(bool &is_logger);
    int GetPageCount() {return page.GetCount();};
    Page* GetPageByIndex(int item) {return page.Item(item);};

    Logger* GetLogger(int index);
    wxWindow* GetWindow(int index);

    void UpdateEffectiveTabOrder(); // refreshes the tab effective order, needed, because tabs might have moved with drag and drop
    void LoadTabOrder(wxString layout);
    wxString SaveTabOrder();

    /*
    *  You should not need to call these functions under normal conditions. The application initialises
    *  everything at startup, after the main window has been created and the plugins have been loaded.
    *  AddLogger:
    *  Add the GUI control obtained by a logger's CreateControl() function.
    *  DeleteLogger:
    *  Remove (delete) a tab and its logger. All log output directed to the corresponging log index
    *  will be redirected to the null log thereafter.
    *  To prove that you are serious, you must know the logger belonging to the tab to delete.
    */
    bool AddLogger(Logger* logger, wxWindow* p, const wxString& title, wxBitmap* icon = 0);
    bool DeleteLogger(Logger* l);

    /*
    *  If something that is not a logger should be placed in the info pane (think twice about this before doing it),
    *  use AddNonLogger()/DeleteNonLogger() for that purpose.
    *  An example of something that is not a logger but might still show up in the info pane is the list of search results.
    */
    bool AddNonLogger(wxWindow* p, const wxString& title, wxBitmap* icon = 0);
    bool RemoveNonLogger(wxWindow* p);
    bool DeleteNonLogger(wxWindow* p);
};


#endif
