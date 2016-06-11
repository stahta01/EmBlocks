/***************************************************************
 * Name:      Navigator
 * Purpose:   Em::Blocks plugin
 * Author:    Gerard Zagema ()
 * Created:   2012-06-04
 * Copyright: Gerard Zagema
 * License:   EmBlocks
 **************************************************************/

#ifndef NAVIGATION_PLUGIN_H_INCLUDED
#define NAVIGATION_PLUGIN_H_INCLUDED

// For compilers that support precompilation, includes <wx/wx.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cbplugin.h> // for "class cbPlugin"
#include <cbstyledtextctrl.h>
#include <editormanager.h>
#include <cbeditor.h>

// we must forward declare the array because it is used inside the class
// declaration
class cbStyledTextCtrl;

typedef struct {
    wxString m_Filename;
    long     m_Posn;
} JumpDataTp;

#define POS_BUFFER_SIZE 40

// ----------------------------------------------------------------------------
class Navigator : public cbPlugin
// ----------------------------------------------------------------------------
{
    public:
        /** Constructor. */
        Navigator();
        /** Destructor. */
        virtual ~Navigator();


        /** This method is called by Em::Blocks and is used by the plugin
          * to add any menu items it needs on Em::Blocks's menu bar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the menu,
          * just do nothing ;)
          * @param menuBar the wxMenuBar to create items in
          */
        void BuildMenu(wxMenuBar* menuBar);

        /** This method is called by Em::Blocks core modules (EditorManager,
          * ProjectManager etc) and is used by the plugin to add any menu
          * items it needs in the module's popup menu. For example, when
          * the user right-clicks on a project file in the project tree,
          * ProjectManager prepares a popup menu to display with context
          * sensitive options for that file. Before it displays this popup
          * menu, it asks all attached plugins (by asking PluginManager to call
          * this method), if they need to add any entries
          * in that menu. This method is called.\n
          * If the plugin does not need to add items in the menu,
          * just do nothing ;)
          * @param type the module that's preparing a popup menu
          * @param menu pointer to the popup menu
          * @param data pointer to FileTreeData object (to access/modify the file tree)
          */
        void BuildModuleMenu(const ModuleType type, wxMenu* menu, const FileTreeData* data = 0);

        /** This method is called by Em::Blocks and is used by the plugin
          * to add any toolbar items it needs on Em::Blocks's toolbar.\n
          * It is a pure virtual method that needs to be implemented by all
          * plugins. If the plugin does not need to add items on the toolbar,
          * just do nothing ;)
          * @param toolBar the wxToolBar to create items on
          * @return The plugin should return true if it needed the toolbar, false if not
          */
        bool BuildToolBar(wxAuiToolBar* toolBar);
    protected:
        /** Any descendent plugin should override this virtual method and
          * perform any necessary initialization. This method is called by
          * Em::Blocks (PluginManager actually) when the plugin has been
          * loaded and should attach in Em::Blocks. When Em::Blocks
          * starts up, it finds and <em>loads</em> all plugins but <em>does
          * not</em> activate (attaches) them. It then activates all plugins
          * that the user has selected to be activated on start-up.\n
          * This means that a plugin might be loaded but <b>not</b> activated...\n
          * Think of this method as the actual constructor...
          */
        void OnAttach();

        /** Any descendent plugin should override this virtual method and
          * perform any necessary de-initialization. This method is called by
          * Em::Blocks (PluginManager actually) when the plugin has been
          * loaded, attached and should de-attach from Em::Blocks.\n
          * Think of this method as the actual destructor...
          * @param appShutDown If true, the application is shutting down. In this
          *         case *don't* use//#include <cbstyledtextctrl.h> Manager::Get()->Get...() functions or the
          *         behaviour is undefined...
          */
        void OnRelease(bool appShutDown);

        // CodeBlocks events
        void OnCursorMove(CodeBlocksEvent& event);
        void OnProjectClosing(CodeBlocksEvent& event);
        void OnProjectActivatedEvent(CodeBlocksEvent& event);

        // Menu events
        void OnMenuJumpBack(wxCommandEvent &event);
        void OnMenuJumpNext(wxCommandEvent &event);

    private:
        void OnUpdateBack(wxUpdateUIEvent& event);
        void OnUpdateNext(wxUpdateUIEvent& event);
        void JumpDataAdd(const wxString& filename, const long posn, const long edlineNum);

        wxLogWindow*    m_pPlgnLog;
        wxString m_FilenameLast;
        long     m_PosnLast;
        int      m_Cursor;
        int      m_Tail;
        int      m_Head;

        wxMenuBar* m_pMenuBar;



        wxAuiToolBar* m_pToolbar;       //!< The plug-in toolbar.

        JumpDataTp m_ArrayOfJumpData[POS_BUFFER_SIZE];

    private:
        DECLARE_EVENT_TABLE();
};

#endif // NAVIGATION_PLUGIN_H_INCLUDED