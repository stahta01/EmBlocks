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

#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <wx/list.h>
#include <wx/treectrl.h>
#include <wx/hashmap.h>
#include <wx/filename.h>

#include "settings.h"
#include "manager.h"
#include "cbexception.h" // cbThrow

#include "printing_types.h"

#include "globals.h" // cbC2U, FileType

DLLIMPORT extern int ID_NBEditorManager;
DLLIMPORT extern int ID_EditorManager;
DLLIMPORT extern int idEditorManagerCheckFiles;


// forward decls
class EditorBase;
class cbAuiNotebook;
class wxAuiNotebookEvent;
class wxMenuBar;
class EditorColourSet;
class cbProject;
class ProjectFile;
class cbEditor;
class cbStyledTextCtrl;
class ListCtrlLogger;
class LoaderBase;
struct EditorManagerInternalData;
class SearchResultsLog;


// forward decl
struct cbFindReplaceData;

/*
 * Struct for store tabs stack info
 */
struct cbNotebookStack
{
    cbNotebookStack(wxWindow* a_pWindow = 0)
        : window (a_pWindow),
          next (0)
   {}

    wxWindow*           window;
    cbNotebookStack*    next;
};

/*
 * No description
 */
class DLLIMPORT EditorManager : public Mgr<EditorManager>, public wxEvtHandler
{
        friend class Mgr<EditorManager>;
        static bool s_CanShutdown;
    public:
        EditorManager& operator=(const EditorManager& /*rhs*/) // prevent assignment operator
        {
        	cbThrow(_T("Can't assign an EditorManager* !!!"));
        	return *this;
		}

        friend class Manager; // give Manager access to our private members
        static bool CanShutdown(){ return s_CanShutdown; }

        cbAuiNotebook* GetNotebook() { return m_pNotebook; }
        cbNotebookStack* GetNotebookStack();
        void DeleteNotebookStack();
        void RebuildNotebookStack();
        void HideCloseButtons(bool hide);

        void CreateMenu(wxMenuBar* menuBar);
        void ReleaseMenu(wxMenuBar* menuBar);
        void Configure();
        int GetEditorsCount();

        EditorBase* IsOpen(const wxString& filename);
        cbEditor* Open(const wxString& filename, int pos = 0,ProjectFile* data = 0);
        cbEditor* Open(LoaderBase* fileLdr, const wxString& filename, int pos = 0,ProjectFile* data = 0);
        EditorBase* GetEditor(int index);
        EditorBase* GetEditor(const wxString& filename){ return IsOpen(filename); } // synonym of IsOpen()
        EditorBase* GetActiveEditor();
        cbEditor* GetBuiltinEditor(EditorBase* eb);
        int FindPageFromEditor(EditorBase* eb);
        void RefreshLanguage();

        // "overloaded" functions for easier access
        // they all return a cbEditor pointer if the editor is builtin, or NULL
        cbEditor* IsBuiltinOpen(const wxString& filename){ return GetBuiltinEditor(IsOpen(filename)); }
        cbEditor* GetBuiltinEditor(int index){ return GetBuiltinEditor(GetEditor(index)); }
        cbEditor* GetBuiltinEditor(const wxString& filename){ return IsBuiltinOpen(filename); } // synonym of IsBuiltinOpen()
        cbEditor* GetBuiltinActiveEditor(){ return GetBuiltinEditor(GetActiveEditor()); }

        // Check if the extension of the filename is marked as internal by the lexers
        bool IsFileMarkedAsInternal(wxString filename);

        void ActivateNext();
        void ActivatePrevious();
        void SetActiveEditor(EditorBase* ed);
        EditorColourSet* GetColourSet(){ return m_Theme; }
        void SetColourSet(EditorColourSet* theme);
        cbEditor* New(const wxString& newFileName = wxEmptyString);

        // these are used *only* for custom editors
        void AddCustomEditor(EditorBase* eb);
        void RemoveCustomEditor(EditorBase* eb);

        bool UpdateProjectFiles(cbProject* project);
        bool SwapActiveHeaderSource();
        bool CloseActive(bool dontsave = false);
        bool Close(const wxString& filename, bool dontsave = false);
        bool Close(EditorBase* editor, bool dontsave = false);
        bool Close(int index, bool dontsave = false);

        // If file is modified, queries to save (yes/no/cancel).
        // Returns false on "cancel".
        bool QueryClose(EditorBase* editor);
        bool QueryCloseAll();
        bool CloseAll(bool dontsave=false);
        bool CloseAllExcept(EditorBase* editor, bool dontsave=false);
        bool CloseTabGroup(EditorBase* editor, bool dontsave=false);
        bool Save(const wxString& filename);
        bool Save(int index);
        bool SaveActive();
        bool SaveAs(int index);
        bool SaveActiveAs();
        bool SaveAll();
        int ShowFindDialog(bool replace,  bool explicitly_find_in_files = false);
        int FindNext(bool goingDown, cbStyledTextCtrl* control = 0, cbFindReplaceData* data = 0);

        void Print(PrintScope ps, PrintColourMode pcm, bool line_numbers);

        /** Hides the editor notebook for layout purposes */
        void HideNotebook();
        /** Shows the previously hidden editor notebook */
        void ShowNotebook();

        /** Restyle all open editors if they need this (or force) */
        void RestyleOpenEditors(bool force = false);

        /** Check if one of the open files has been modified outside the IDE. If so, ask to reload it. */
        void CheckForExternallyModifiedFiles();

        SearchResultsLog* GetSearchResultLogger() const { return m_pSearchLog; }

        void OnGenericContextMenuHandler(wxCommandEvent& event);
        void OnPageChanged(wxAuiNotebookEvent& event);
        void OnPageChanging(wxAuiNotebookEvent& event);
        void OnPageClose(wxAuiNotebookEvent& event);
        void OnPageContextMenu(wxAuiNotebookEvent& event);
        void OnClose(wxCommandEvent& event);
        void OnCloseAll(wxCommandEvent& event);
        void OnCloseAllOthers(wxCommandEvent& event);
        void OnCloseTabGroup(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void OnSaveAll(wxCommandEvent& event);
        void OnSwapHeaderSource(wxCommandEvent& event);
        void OnAddFileToProject(wxCommandEvent& event);
        void OnShowFileInTree(wxCommandEvent& event);
        void OnShowFileInFolder(wxCommandEvent& event);
        void OnAppDoneStartup(wxCommandEvent& event);
        void OnAppStartShutdown(wxCommandEvent& event);
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnTreeItemSelected(wxTreeEvent &event);
        void OnTreeItemActivated(wxTreeEvent &event);
        void OnTreeItemRightClick(wxTreeEvent &event);
        void SetZoom(int zoom);
        int GetZoom()const;

    protected:
        // m_EditorsList access
        void AddEditorBase(EditorBase* eb);
        void RemoveEditorBase(EditorBase* eb);
        cbEditor* InternalGetBuiltinEditor(int page);
        EditorBase* InternalGetEditorBase(int page);

        void CreateSearchLog();
        void LogSearch(const wxString& file, int line, const wxString& lineText);

    private:
        EditorManager(const EditorManager& /*rhs*/); // prevent copy construction

        EditorManager();
        ~EditorManager();
        void CalculateFindReplaceStartEnd(cbStyledTextCtrl* control, cbFindReplaceData* data, bool replace = false);
        void OnCheckForModifiedFiles(wxCommandEvent& event);
        int Find(cbStyledTextCtrl* control, cbFindReplaceData* data);
        int FindInFiles(cbFindReplaceData* data);
        int Replace(cbStyledTextCtrl* control, cbFindReplaceData* data);
        int ReplaceInFiles(cbFindReplaceData* data);
        bool IsHeaderSource(const wxFileName& candidateFile, const wxFileName& activeFile, FileType ftActive);
        wxFileName FindHeaderSource(const wxArrayString& candidateFilesArray, const wxFileName& activeFile, bool& isCandidate);

        wxBitmap m_bmpSplitVert;
        wxBitmap m_bmpSplitHorz;

        cbAuiNotebook*             m_pNotebook;
        cbNotebookStack*           m_pNotebookStackHead;
        cbNotebookStack*           m_pNotebookStackTail;
        size_t                     m_nNotebookStackSize;
        cbFindReplaceData*         m_LastFindReplaceData;
        EditorColourSet*           m_Theme;
        SearchResultsLog*          m_pSearchLog;
        int                        m_SearchLogIndex;
        int                        m_Zoom;
        bool                       m_isCheckingForExternallyModifiedFiles;
        friend struct              EditorManagerInternalData;
        EditorManagerInternalData* m_pData;
        EditorBase*                m_prevEditor;

        DECLARE_EVENT_TABLE()
};

#endif // EDITORMANAGER_H

