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

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "sdk_precomp.h"

#ifndef WX_PRECOMP
#include "wx/utils.h"
#include "wx/dialog.h"
#include "wx/log.h"
#include "wx/app.h"     // for GetComCtl32Version()
#endif

#include "wx/msw/private.h"
#include "wx/msw/wrapshl.h"


// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif

#ifndef BIF_NONEWFOLDERBUTTON
#define BIF_NONEWFOLDERBUTTON  0x0200
#endif

#ifndef BIF_EDITBOX
#define BIF_EDITBOX 16
#endif

// ----------------------------------------------------------------------------
// wxWidgets macros
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(SelectDirDialog, wxDirDialogBase)

// ----------------------------------------------------------------------------
// private functions prototypes
// ----------------------------------------------------------------------------

// the callback proc for the dir dlg
static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp,
                                       LPARAM pData);


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// SelectDirDialog
// ----------------------------------------------------------------------------

SelectDirDialog::SelectDirDialog(wxWindow *parent,
                                 const wxString& name,
                                 const wxString& message,
                                 const wxString& defaultPath,
                                 const wxString& searchFile,
                                 const wxBitmap& bitmap)
{
    m_message = message;
    m_parent = parent;
    m_name = name;
    m_searchFile = searchFile;
    m_bitmap = bitmap;

    SetWindowStyle(wxDD_DEFAULT_STYLE);
    SetPath(defaultPath);
}

void SelectDirDialog::SetPath(const wxString& path)
{
    m_path = path;

    // SHBrowseForFolder doesn't like '/'s nor the trailing backslashes
    m_path.Replace(_T("/"), _T("\\"));
    if ( !m_path.empty() )
    {
        while ( *(m_path.end() - 1) == _T('\\') )
        {
            m_path.erase(m_path.length() - 1);
        }

        // but the root drive should have a trailing slash (again, this is just
        // the way the native dialog works)
        if ( *(m_path.end() - 1) == _T(':') )
        {
            m_path += _T('\\');
        }
    }
}

int SelectDirDialog::ShowModal()
{
    wxWindow *parent = GetParent();

    BROWSEINFO bi;
    bi.hwndOwner      = parent ? GetHwndOf(parent) : NULL;
    bi.pidlRoot       = NULL;
    bi.pszDisplayName = NULL;
    // Please don't change this without checking it compiles
    // with eVC++ first.

    bi.lpszTitle      = m_message.c_str();
    bi.ulFlags        = BIF_RETURNONLYFSDIRS;
    bi.lpfn           = BrowseCallbackProc;
    bi.lParam         = (LPARAM)this;    // param for the callback


//  bi.ulFlags |= BIF_EDITBOX;
    // do show the dialog
    wxItemIdList pidl(SHBrowseForFolder(&bi));
    wxItemIdList::Free((LPITEMIDLIST)bi.pidlRoot);

    if ( !pidl )
    {
        // Cancel button pressed
        return wxID_CANCEL;
    }

    m_path = pidl.GetPath();

    // change current working directory if asked so
    if (HasFlag(wxDD_CHANGE_DIR))
        wxSetWorkingDirectory(m_path);

    return m_path.empty() ? wxID_CANCEL : wxID_OK;
}

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------
static int CALLBACK
BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    SelectDirDialog* pDlg = (SelectDirDialog*)pData;

    switch(uMsg)
    {

    case BFFM_INITIALIZED:
    {
        // sent immediately after initialisation and so we may set the
        // initial selection here
        //
        // wParam = TRUE => lParam is a string and not a PIDL
        ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pDlg->GetPath().c_str());
        SetWindowText(hwnd, pDlg->GetName().wx_str() );



        if( wxFindFirstFile(pDlg->GetPath() + pDlg->GetSearchFile() ).IsEmpty()  )
            SendMessage(hwnd, BFFM_ENABLEOK, 0, 0);

        wxIcon icon;
        icon.CopyFromBitmap( pDlg->GetBitmap() );
        if(icon.IsOk())
            ::SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)icon.GetHICON());
    }
    break;



    case BFFM_SELCHANGED:
    {
        // Set the status window to the currently selected path.
        wxString strDir;
        if ( SHGetPathFromIDList((LPITEMIDLIST)lp,
                                 wxStringBuffer(strDir, MAX_PATH)) )
        {
            if( wxFindFirstFile(strDir+ pDlg->GetSearchFile()).IsEmpty()  )
                SendMessage(hwnd, BFFM_ENABLEOK, 0, 0);
        }
    }
    break;

    //case BFFM_VALIDATEFAILED: -- might be used to provide custom message
    //                             if the user types in invalid dir name
    }

    return 0;
}

