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


    @version $Revision: 30 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-26 19:01:36 +0100 (Tue, 26 Nov 2013) $:

 */
#ifndef MAIN_H
#define MAIN_H

#include <map>

#include <wx/aui/aui.h> // wxAuiManager
#include <wx/aui/auibar.h> // wxAuiManager
//#include <wx/toolbar.h>
#include <wx/docview.h> // for wxFileHistory
#include <wx/notebook.h>
#include <wx/dynarray.h>
#include <cbeditor.h>
#include "manager.h"
#include "cbexception.h"
#include "cbplugin.h"
#include "sdk_events.h"
#include "scripting/bindings/sc_base_types.h"
#include "scrollingdialog.h"
#include "rssreader.h"

WX_DECLARE_HASH_MAP(int, wxString, wxIntegerHash, wxIntegerEqual, PluginIDsMap);
WX_DECLARE_HASH_MAP(cbPlugin*, wxAuiToolBar*, wxPointerHash, wxPointerEqual, PluginToolbarsMap);
WX_DECLARE_STRING_HASH_MAP(wxString, LayoutViewsMap);

extern int idStartHerePageLink;
extern int idStartHerePageVarSubst;

class cbAuiNotebook;
class InfoPane;
class wxGauge;


class MainFrame : public wxFrame
{
	public:
        // needed for binding with SqPlus
        MainFrame& operator=(const MainFrame& /*rhs*/) // prevent assignement operator
        {
        	cbThrow(_T("Can't use MainFrame's operator="));
        	return *this;
		}
    private:
        MainFrame(const MainFrame& /*rhs*/); // prevent copy construction

        wxAuiManager m_LayoutManager;
        LayoutViewsMap m_LayoutViews;
        LayoutViewsMap m_LayoutMessagePane;
        bool LayoutDifferent(const wxString& layout1,const wxString& layout2,const wxString& delimiter=_("|"));
        bool LayoutMessagePaneDifferent(const wxString& layout1,const wxString& layout2, bool checkSelection=false);
    public:
        wxAcceleratorTable* m_pAccel;
        MainFrame(wxWindow* parent = (wxWindow*)NULL);
        ~MainFrame();

        wxAuiManager *GetLayoutManager() {return &m_LayoutManager;}
        bool Open(const wxString& filename, bool addToHistory = true);
        bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
        void ShowTips(bool forceShow = false);
        bool IsShutdown(void) {return m_InitiatedShutdown;}

        // This function tests if the window is really on screen even if the application
        // is resized.
        bool IsWindowReallyVisible(wxWindow* win);

        wxScrollingDialog* GetBatchBuildDialog(){ return m_pBatchBuildDialog; }

        // show a file-open dialog and return the selection
        wxString ShowOpenFileDialog(const wxString& caption, const wxString& filter);
        // open the filename (based on what it is)
        bool OpenGeneric(const wxString& filename, bool addToHistory = true);

        // event handlers

        void OnEraseBackground(wxEraseEvent& event);
        void OnSize(wxSizeEvent& event);
        void OnApplicationClose(wxCloseEvent& event);
        void OnStartHereLink(wxCommandEvent& event);
        void OnStartHereVarSubst(wxCommandEvent& event);

        // File->New submenu entries handler
        void OnFileNewWhat(wxCommandEvent& event);

        void OnFileNew(wxCommandEvent& event);
        void OnFileOpen(wxCommandEvent& event);
        void OnFileReopenProject(wxCommandEvent& event);
        void OnFileOpenRecentProjectClearHistory(wxCommandEvent& event);
        void OnFileReopen(wxCommandEvent& event);
        void OnFileOpenRecentClearHistory(wxCommandEvent& event);
        void OnFileImportProjectDevCpp(wxCommandEvent& event);
        void OnFileSave(wxCommandEvent& event);
        void OnFileSaveAs(wxCommandEvent& event);
        void OnFileSaveAllFiles(wxCommandEvent& event);
        void OnFileSaveProject(wxCommandEvent& event);
        void OnFileSaveProjectAs(wxCommandEvent& event);
        void OnFileSaveProjectTemplate(wxCommandEvent& event);
        void OnFileSaveProjectAllProjects(wxCommandEvent& event);
        void OnFileOpenDefWorkspace(wxCommandEvent& event);
        void OnFileSaveWorkspace(wxCommandEvent& event);
        void OnFileSaveWorkspaceAs(wxCommandEvent& event);
        void OnFileSaveAll(wxCommandEvent& event);
        void OnFileCloseWorkspace(wxCommandEvent& event);
        void OnFileClose(wxCommandEvent& event);
        void OnFileCloseAll(wxCommandEvent& event);
        void OnFileCloseProject(wxCommandEvent& event);
        void OnFileCloseAllProjects(wxCommandEvent& event);
        void OnFilePrintSetup(wxCommandEvent& event);
        void OnFilePrint(wxCommandEvent& event);
        void OnFileQuit(wxCommandEvent& event);
        void OnFileNext(wxCommandEvent& event);
        void OnFilePrev(wxCommandEvent& event);

        void OnEditUndo(wxCommandEvent& event);
        void OnEditRedo(wxCommandEvent& event);
        void OnEditClearHistory(wxCommandEvent& event);
        void OnEditCopy(wxCommandEvent& event);
        void OnEditCut(wxCommandEvent& event);
        void OnEditPaste(wxCommandEvent& event);
        void OnEditSwapHeaderSource(wxCommandEvent& event);
        void OnEditGotoMatchingBrace(wxCommandEvent& event);
        void OnEditHighlightMode(wxCommandEvent& event);
        void OnEditFoldAll(wxCommandEvent& event);
        void OnEditUnfoldAll(wxCommandEvent& event);
        void OnEditToggleFoldBlock(wxCommandEvent& event);
        void OnEditEOLMode(wxCommandEvent& event);
        void OnEditEncoding(wxCommandEvent& event);
        void OnEditParaUp(wxCommandEvent& event);
        void OnEditParaUpExtend(wxCommandEvent& event);
        void OnEditParaDown(wxCommandEvent& event);
        void OnEditParaDownExtend(wxCommandEvent& event);
        void OnEditWordPartLeft(wxCommandEvent& event);
        void OnEditWordPartLeftExtend(wxCommandEvent& event);
        void OnEditWordPartRight(wxCommandEvent& event);
        void OnEditWordPartRightExtend(wxCommandEvent& event);
        void OnEditZoomIn(wxCommandEvent& event);
        void OnEditZoomOut(wxCommandEvent& event);
        void OnEditZoomReset(wxCommandEvent& event);
        void OnEditLineCut(wxCommandEvent& event);
        void OnEditLineDelete(wxCommandEvent& event);
        void OnEditLineDuplicate(wxCommandEvent& event);
        void OnEditLineTranspose(wxCommandEvent& event);
        void OnEditLineCopy(wxCommandEvent& event);
        void OnEditLinePaste(wxCommandEvent& event);
        void OnEditUpperCase(wxCommandEvent& event);
        void OnEditLowerCase(wxCommandEvent& event);
        void OnEditInsertNewLine(wxCommandEvent& event);
        void OnEditGotoLineEnd(wxCommandEvent& event);
        void OnEditSelectAll(wxCommandEvent& event);
        void OnEditCommentSelected(wxCommandEvent& event);
        void OnEditUncommentSelected(wxCommandEvent& event);
        void OnEditToggleCommentSelected(wxCommandEvent & event);
        void OnEditStreamCommentSelected(wxCommandEvent & event);
        void OnEditBoxCommentSelected(wxCommandEvent & event);

        void OnEditBookmarksToggle(wxCommandEvent& event);
        void OnEditBookmarksNext(wxCommandEvent& event);
        void OnEditBookmarksPrevious(wxCommandEvent& event);

        void OnViewLayout(wxCommandEvent& event);
        void OnViewLayoutSaveAs(wxCommandEvent& event);
        void OnViewLayoutSave(wxCommandEvent& event);
        void OnViewLayoutDelete(wxCommandEvent& event);
        void OnViewScriptConsole(wxCommandEvent& event);

        void OnToggleBookmark(wxCommandEvent& event);
        void OnPrevBookmark(wxCommandEvent& event);
        void OnNextBookmark(wxCommandEvent& event);
        void OnClearBookmark(wxCommandEvent& event);

        void OnSearchFind(wxCommandEvent& event);
        void OnSearchFindNext(wxCommandEvent& event);
        void OnSearchReplace(wxCommandEvent& event);
        void OnSearchGotoLine(wxCommandEvent& event);
        void OnSearchGotoNextChanged(wxCommandEvent& event);
        void OnSearchGotoPrevChanged(wxCommandEvent& event);

        void OnPluginsExecuteMenu(wxCommandEvent& event);

        void OnSettingsEnvironment(wxCommandEvent& event);
        void OnSettingsKeyBindings(wxCommandEvent& event);
        void OnGlobalUserVars(wxCommandEvent& event);
        void OnSettingsEditor(wxCommandEvent& event);
        void OnSettingsTools(wxCommandEvent& event);
        void OnSettingsPlugins(wxCommandEvent& event);
        void OnPluginSettingsMenu(wxCommandEvent& event);
        void OnSettingsScripting(wxCommandEvent& event);

        void OnHelpAbout(wxCommandEvent& event);
        void OnHelpTips(wxCommandEvent& event);
        void OnHelpPluginMenu(wxCommandEvent& event);

        void OnToggleBar(wxCommandEvent& event);
        void OnToggleStatusBar(wxCommandEvent& event);
        void OnFocusLogsAndOthers(wxCommandEvent& event);
        void OnSwitchTabs(wxCommandEvent& event);
        void OnToggleFullScreen(wxCommandEvent& event);

        void OnMenu(wxCommandEvent& event);

        // plugin events
        void OnPluginLoaded(CodeBlocksEvent& event);
        void OnPluginUnloaded(CodeBlocksEvent& event);
        void OnPluginInstalled(CodeBlocksEvent& event);
        void OnPluginUninstalled(CodeBlocksEvent& event);

        // general UpdateUI events
        void OnEditorUpdateUI(CodeBlocksEvent& event);
        void OnFileMenuUpdateUI(wxUpdateUIEvent& event);
        void OnEditMenuUpdateUI(wxUpdateUIEvent& event);
        void OnViewMenuUpdateUI(wxUpdateUIEvent& event);
        void OnSearchMenuUpdateUI(wxUpdateUIEvent& event);
        void OnProjectMenuUpdateUI(wxUpdateUIEvent& event);
        void OnBookmarkMenuUpdateUI(wxUpdateUIEvent& event);

        // project events
        void OnProjectActivated(CodeBlocksEvent& event);
        void OnProjectOpened(CodeBlocksEvent& event);
        void OnProjectClosed(CodeBlocksEvent& event);

        // dock/undock window requests
        void OnRequestDockWindow(CodeBlocksDockEvent& event);
        void OnRequestUndockWindow(CodeBlocksDockEvent& event);
        void OnRequestShowDockWindow(CodeBlocksDockEvent& event);
        void OnRequestHideDockWindow(CodeBlocksDockEvent& event);
        void OnDockWindowVisibility(CodeBlocksDockEvent& event);

        // layout requests
        void OnLayoutUpdate(CodeBlocksLayoutEvent& event);
        void OnLayoutQuery(CodeBlocksLayoutEvent& event);
        void OnLayoutSwitch(CodeBlocksLayoutEvent& event);

        // log requests
        void OnAddLogWindow(CodeBlocksLogEvent& event);
        void OnRemoveLogWindow(CodeBlocksLogEvent& event);
        void OnHideLogWindow(CodeBlocksLogEvent& event);
        void OnSwitchToLogWindow(CodeBlocksLogEvent& event);
        void OnGetActiveLogWindow(CodeBlocksLogEvent& event);
        void OnShowLogManager(CodeBlocksLogEvent& event);
        void OnHideLogManager(CodeBlocksLogEvent& event);
        void OnLockLogManager(CodeBlocksLogEvent& event);
        void OnUnlockLogManager(CodeBlocksLogEvent& event);
        void OnToggleLogView(CodeBlocksLogEvent& event);

        // editor changed events
        void OnEditorOpened(CodeBlocksEvent& event);
        void OnEditorActivated(CodeBlocksEvent& event);
        void OnEditorClosed(CodeBlocksEvent& event);
        void OnEditorSaved(CodeBlocksEvent& event);
        void OnEditorModified(CodeBlocksEvent& event);
        void OnPageChanged(wxNotebookEvent& event);
        void OnShiftTab(wxCommandEvent& event);
        void OnCtrlAltTab(wxCommandEvent& event);
        void StartupDone();
        void OnNotebookDoubleClick(CodeBlocksEvent& /*event*/);

        // Rss feed update
        void OnRssFeedUpdate(CodeBlocksEvent& event);
        void OnStartPageUpdate(CodeBlocksEvent& event);

    protected:
        void CreateIDE();
        void CreateMenubar();
        void CreateToolbars();
        void ScanForPlugins();
        void RecreateMenuBar();
        void RegisterEvents();
        void SetupGUILogging();
        void BuildLogViewMenu();

        void RegisterScriptFunctions();
        void RunStartupScripts();

        enum { Installed, Uninstalled, Unloaded };
        void PluginsUpdated(cbPlugin* plugin, int status);

        void DoAddPlugin(cbPlugin* plugin);
        void DoAddPluginToolbar(cbPlugin* plugin);
        void DoAddPluginStatusField(cbPlugin* plugin);
        void AddPluginInPluginsMenu(cbPlugin* plugin);
        void AddPluginInSettingsMenu(cbPlugin* plugin);
        void AddPluginInHelpPluginsMenu(cbPlugin* plugin);
        wxMenuItem* AddPluginInMenus(wxMenu* menu, cbPlugin* plugin, wxObjectEventFunction callback, int pos = -1, bool checkable = false);
        void RemovePluginFromMenus(const wxString& pluginName);

        void LoadViewLayout(const wxString& name, bool isTemp = false);
        void SaveViewLayout(const wxString& name, const wxString& layout, const wxString& layoutMP, bool select = false);
        void DoSelectLayout(const wxString& name);
        void DoFixToolbarsLayout();
        bool DoCheckCurrentLayoutForChanges(bool canCancel = true);

        void AskToRemoveFileFromHistory(wxFileHistory* hist, int id, bool cannot_open = true);

        void AddEditorInWindowMenu(const wxString& filename, const wxString& title);
        void RemoveEditorFromWindowMenu(const wxString& filename);
        int IsEditorInWindowMenu(const wxString& filename);
        wxString GetEditorDescription(EditorBase* eb);

        bool DoCloseCurrentWorkspace();
        bool DoOpenProject(const wxString& filename, bool addToHistory = true);
        bool DoOpenFile(const wxString& filename, bool addToHistory = true);
        void DoOnFileOpen();

        void DoCreateStatusBar();
        void DoUpdateStatusBar();
        void DoUpdateAppTitle();
        void DoUpdateLayout();
        void DoUpdateLayoutColours();
        void DoUpdateEditorStyle();
        void DoUpdateEditorStyle(cbAuiNotebook* target, const wxString& prefix, long defaultStyle);

        void ShowHideStartPage();
        void ShowHideScriptConsole();

        void LoadWindowState();
        void SaveWindowState();

        void InitializeRecentFilesHistory();
        void AddToRecentFilesHistory(const wxString& filename);
        void AddToRecentProjectsHistory(const wxString& filename);
        void TerminateRecentFilesHistory();
        #if wxUSE_STATUSBAR
        virtual wxStatusBar *OnCreateStatusBar(int number, long style, wxWindowID id, const wxString& name);
        #endif

        wxFileHistory* m_pFilesHistory;
        wxFileHistory* m_pProjectsHistory;

        /// "Close FullScreen" button. Only shown when in FullScreen view
        wxButton* m_pCloseFullScreenBtn;

        EditorManager*  m_pEdMan;
        ProjectManager* m_pPrjMan;
        LogManager*     m_pLogMan;
        InfoPane*       m_pInfoPane;

        wxAuiToolBar* m_pToolbar;
        wxAuiToolBar* m_pBookmarkBar;
        PluginToolbarsMap m_PluginsTools;

        PluginIDsMap m_PluginIDsMap;
        wxMenu* m_ToolsMenu;
        wxMenu* m_PluginsMenu;
        wxMenu* m_HelpPluginsMenu;

        bool m_StartupDone;
        bool m_InitiatedShutdown;

        int m_AutoHideLockCounter;
        int m_LastCtrlAltTabWindow; //!< Last window focussed in the cycle 1 = Mgmt. panel, 2 = Editor, 3 = Logs & others

        wxString m_PreviousLayoutName;
        wxString m_LastLayoutName;
        wxString m_LastLayoutData;
        wxString m_LastMessagePaneLayoutData;
        bool m_LastLayoutIsTemp;

        wxWindow* m_pScriptConsole;

        typedef std::map<int, const wxString> MenuIDToScript; // script menuitem ID -> script function name
        MenuIDToScript m_MenuIDToScript;

        wxScrollingDialog* m_pBatchBuildDialog;
        wxGauge* m_pProgressBar;

        EditorBase* m_pStartHerePage;
        wxRSSReader m_RssReader;

        DECLARE_EVENT_TABLE()
};

#endif // MAIN_H

