// ----------------------------------------------------------------------------
//  Navigation.cpp
// ----------------------------------------------------------------------------
#include <sdk.h> // Code::Blocks SDK

#include "Version.h"
#include "Navigation.h"
#include <sdk_events.h>

#include <editor_hooks.h>


// ----------------------------------------------------------------------------
//  namespace
// ----------------------------------------------------------------------------
// Register the plugin with Code::Blocks.
// We are using an anonymous namespace so we don't litter the global one.
namespace
{
    PluginRegistrant<Navigator> reg(_T("Navigation"));
    int idMenuJumpView = wxNewId();
    int idMenuJumpBack = XRCID("ID_NAV_BACKWARD");
    int idMenuJumpNext = XRCID("ID_NAV_FORWARD");
}

// ----------------------------------------------------------------------------
//  Events Table
// ----------------------------------------------------------------------------
// events handling
BEGIN_EVENT_TABLE(Navigator, cbPlugin)
    EVT_UPDATE_UI(idMenuJumpBack, Navigator::OnUpdateBack)
    EVT_MENU( idMenuJumpBack,  Navigator::OnMenuJumpBack)

    EVT_UPDATE_UI(idMenuJumpNext, Navigator::OnUpdateNext)
    EVT_MENU( idMenuJumpNext,  Navigator::OnMenuJumpNext)
END_EVENT_TABLE()

// constructor
// ----------------------------------------------------------------------------
Navigator::Navigator()
// ----------------------------------------------------------------------------
{
    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
//    if(!Manager::LoadResource(_T("Navigator.zip")))
//    {
//        NotifyMissingFile(_T("Navigator.zip"));
//    }

    m_FilenameLast = wxEmptyString;
    m_PosnLast = 0;

    m_Cursor = POS_BUFFER_SIZE-1;
    m_Tail = POS_BUFFER_SIZE-1;
    m_Head = POS_BUFFER_SIZE-1;

    // Make sure our resources are available.
    // In the generated boilerplate code we have no resources but when
    // we add some, it will be nice that this code is in place already ;)
    if(!Manager::LoadResource(wxT("Navigation.zip"))){
        NotifyMissingFile(wxT("Navigation.zip"));
    }


    m_IsAttached = false;
}
// ----------------------------------------------------------------------------
Navigator::~Navigator()
// ----------------------------------------------------------------------------
{
    // destructor
}

// ----------------------------------------------------------------------------
void Navigator::OnAttach()
// ----------------------------------------------------------------------------
{
    // Codeblocks Events registration

    m_IsAttached = true;

    // -- Project events
    Manager::Get()->RegisterEventSink(cbEVT_PROJECT_ACTIVATE, new cbEventFunctor<Navigator, CodeBlocksEvent>(this, &Navigator::OnProjectActivatedEvent));
    Manager::Get()->RegisterEventSink(cbEVT_PROJECT_CLOSE, new cbEventFunctor<Navigator, CodeBlocksEvent>(this, &Navigator::OnProjectClosing));
    // -- Navigation events
    Manager::Get()->RegisterEventSink(cbEVT_CURSOR_POS_SAVE, new cbEventFunctor<Navigator, CodeBlocksEvent>(this, &Navigator::OnCursorMove));
}

// ----------------------------------------------------------------------------
void Navigator::OnRelease(bool /*appShutDown*/)
// ----------------------------------------------------------------------------
{
    // do de-initialization for your plugin
    // if appShutDown is true, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // NOTE: after this function, the inherited member variable

    m_IsAttached = false;
}
// ----------------------------------------------------------------------------
void Navigator::BuildMenu(wxMenuBar* menuBar)
// ----------------------------------------------------------------------------
{
    //The application is offering its menubar for your plugin,
    //to add any menu items you want...
    //Append any items you need in the menu...
    //NOTE: Be careful in here... The application's menubar is at your disposal.
    //-NotImplemented(_T("Navigator::BuildMenu()"));

    m_pMenuBar = menuBar;


    int idx = menuBar->FindMenu(_("&Edit"));
    if (idx != wxNOT_FOUND)
    {
        wxMenu* edit = menuBar->GetMenu(idx);
        edit->AppendSeparator();
        edit->Append(idMenuJumpBack,  _("Navigation Backwards\tAlt-LEFT"), _("Back to previous position"));
        edit->Append(idMenuJumpNext,  _("Navigation Forwards\tAlt-RIGHT"),  _("Forward to next position"));
    }
}
// ----------------------------------------------------------------------------
void Navigator::BuildModuleMenu(const ModuleType /*type*/, wxMenu* /*menu*/, const FileTreeData* /*data*/)
// ----------------------------------------------------------------------------
{
    //Some library module is ready to display a pop-up menu.
    //Check the parameter \"type\" and see which module it is
    //and append any items you need in the menu...
    //TIP: for consistency, add a separator as the first item...
    //-NotImplemented(_T("Navigator::BuildModuleMenu()"));
}
// ----------------------------------------------------------------------------
bool Navigator::BuildToolBar(wxAuiToolBar *toolBar)
// ----------------------------------------------------------------------------
{
    if(!IsAttached() || !toolBar)
        return false;

    Manager::Get()->AuiToolBar(toolBar,_T("navigation_toolbar"));
    toolBar->Realize();
    m_pToolbar = toolBar;

#if wxCHECK_VERSION(2, 8, 0)
    m_pToolbar->SetInitialSize();
#else
    m_pToolbar->SetBestFittingSize();
#endif

    // Make toolbar visible at initial launch (without perspective's)
    toolBar->SetClientData((void*)true);

    Connect(idMenuJumpBack, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&Navigator::OnMenuJumpBack);
    Connect(idMenuJumpNext, wxEVT_COMMAND_TOOL_CLICKED, (wxObjectEventFunction)&Navigator::OnMenuJumpNext);
    return true;
}
// ----------------------------------------------------------------------------
void Navigator::OnCursorMove(CodeBlocksEvent& event)
// ----------------------------------------------------------------------------
{
    event.Skip();

    cbEditor* ed =  Manager::Get()->GetEditorManager()->GetBuiltinActiveEditor();
    if(not ed)
        return;

    wxString edFilename = ed->GetFilename();
    cbStyledTextCtrl* edstc = ed->GetControl();
    if(edstc->GetCurrentLine() == wxSCI_INVALID_POSITION)
        return;

    long edLine = edstc->GetCurrentLine();
    long edPosn = edstc->GetCurrentPos();

    JumpDataAdd(edFilename, edPosn, edLine );
}//OnEditorUpdateEvent


// ----------------------------------------------------------------------------
void Navigator::OnProjectClosing(CodeBlocksEvent& event)
// ----------------------------------------------------------------------------
{
    event.Skip();
    m_Cursor = POS_BUFFER_SIZE-1;
    m_Tail = POS_BUFFER_SIZE-1;
    m_Head = POS_BUFFER_SIZE-1;
}
// ----------------------------------------------------------------------------
void Navigator::OnProjectActivatedEvent(CodeBlocksEvent& event)
// ----------------------------------------------------------------------------
{
    event.Skip();
}//OnProjectActivatedEvent

// ----------------------------------------------------------------------------
void Navigator::JumpDataAdd(const wxString& filename, const long posn, const long lineNum)
// ----------------------------------------------------------------------------
{
    // Skip this position if it was equal to the previous on line number level.
    if( (m_ArrayOfJumpData[m_Cursor].m_Filename == filename) &&
        (m_ArrayOfJumpData[m_Cursor].m_Posn  == posn ) )
            return;

    if ( ++m_Cursor >= POS_BUFFER_SIZE )
        m_Cursor = 0;

    m_Head = m_Cursor;

    m_ArrayOfJumpData[m_Cursor].m_Filename = filename;
    m_ArrayOfJumpData[m_Cursor].m_Posn     = posn;

    if(m_Head == m_Tail)
    {
        m_Tail++;
        if ( m_Tail >= POS_BUFFER_SIZE )
            m_Tail = 0;
    }

    #if defined(LOGGING)
    LOGIT( _T("JT JumpDataAdd[%s][%ld][%d]"), filename.c_str(), posn, m_head);
    #endif

}

// ----------------------------------------------------------------------------
void Navigator::OnMenuJumpBack(wxCommandEvent &/*event*/)
// ----------------------------------------------------------------------------
{
    int tailPlusOne;

    #if defined(LOGGING)
    LOGIT( _T("JT [%s]"), _T("OnMenuJumpBack"));
    #endif


    tailPlusOne = m_Tail+1;
    if(tailPlusOne >= POS_BUFFER_SIZE )
        tailPlusOne = 0;


    // Can we go further back
    if( m_Cursor == tailPlusOne)
        return;

    m_Cursor--;


    if(m_Cursor == -1)
        m_Cursor = POS_BUFFER_SIZE-1;

    EditorManager* edmgr = Manager::Get()->GetEditorManager();

    wxString edFilename = m_ArrayOfJumpData[m_Cursor].m_Filename;
    long edPosn = m_ArrayOfJumpData[m_Cursor].m_Posn;


    #if defined(LOGGING)
        LOGIT( _T("JT OnMenuJumpBack [%s][%ld]curs[%d]"), edFilename.c_str(), edPosn, m_Cursor);
    #endif

    // activate editor
    cbEditor* ed = edmgr->Open(edFilename);
    if(ed)
    {
        cbStyledTextCtrl* control = ed->GetControl();
        ed->GotoLine(ed->GetControl()->LineFromPosition(edPosn)); //center on scrn
        ed->SetFocus();
        control->GotoPos(edPosn);
    }

    #if defined(LOGGING)
    LOGIT( _T("JT [%s]"), _T("END OnMenuJumpBack"));
    #endif
}

// ----------------------------------------------------------------------------
void Navigator::OnUpdateBack(wxUpdateUIEvent& event)
// ----------------------------------------------------------------------------
{
    int tailPlusOne;

    tailPlusOne = m_Tail+1;
    if(tailPlusOne >= POS_BUFFER_SIZE )
        tailPlusOne = 0;

    event.Enable( (m_Tail!=m_Head) && (m_Cursor!=tailPlusOne) );
}


// ----------------------------------------------------------------------------
void Navigator::OnMenuJumpNext(wxCommandEvent &/*event*/)
// ----------------------------------------------------------------------------
{
    #if defined(LOGGING)
    LOGIT( _T("JT [%s]"), _T("OnMenuJumpNext"));
    #endif


    // Can we go further back
   if( m_Cursor == m_Head)
       return;

    m_Cursor++;

    if(m_Cursor >= POS_BUFFER_SIZE)
        m_Cursor = 0;

    EditorManager* edmgr = Manager::Get()->GetEditorManager();

    wxString edFilename = m_ArrayOfJumpData[m_Cursor].m_Filename;
    long edPosn = m_ArrayOfJumpData[m_Cursor].m_Posn;

    #if defined(LOGGING)
        LOGIT( _T("JT OnMenuJumpBack [%s][%ld]curs[%d]"), edFilename.c_str(), edPosn, m_Cursor);
    #endif

    // activate editor
    cbEditor* ed = edmgr->Open(edFilename);
    if(ed)
    {
        cbStyledTextCtrl* control = ed->GetControl();
        ed->GotoLine(ed->GetControl()->LineFromPosition(edPosn)); //center on scrn
        ed->SetFocus();
        control->GotoPos(edPosn);
    }


    #if defined(LOGGING)
    LOGIT( _T("JT [%s]"), _T("END OnMenuJumpBack"));
    #endif
}

// ----------------------------------------------------------------------------
void Navigator::OnUpdateNext(wxUpdateUIEvent& event)
// ----------------------------------------------------------------------------
{
    event.Enable(m_Cursor!=m_Head);
}

