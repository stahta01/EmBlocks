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
#ifndef ASSOCIATIONS_H
#define ASSOCIATIONS_H

#include "scrollingdialog.h"
#include <wx/intl.h>
#include <wx/string.h>
#ifdef __WXMSW__
    #include <wx/msw/registry.h>
    #include <shlobj.h> // for SHChangeNotify()
    #ifdef CONSOLE_DEBUG
        #include <windows.h>
        #include <wincon.h>
    #endif
    #define DDE_SERVICE    _T("EMBLOCKS")
#else
    #define DDE_SERVICE    _T("/tmp/EMBLOCKS%s.socket")
#endif
#define DDE_TOPIC    _T("EmBlocksDDEServer")
#include <wx/ipc.h>

class wxCheckListBox;
class wxWindow;
class wxCommandEvent;

namespace Associations
{
    struct Assoc
    {
    wxString ext;
    wxString descr;
    int index;
    };

    void SetCore();
    void SetAll();
    void ClearAll();
    void SetBatchBuildOnly();
    bool Check();

    void DoSetAssociation(const wxString& ext, const wxString& descr, const wxString& exe, int icoNum);
    bool DoCheckAssociation(const wxString& ext, const wxString& descr, const wxString& exe, int icoNum);
    void DoClearAssociation(const wxString& ext);

    void UpdateChanges();
};

class ManageAssocsDialog : public wxScrollingDialog
{
    wxCheckListBox* list;

    public:
        ManageAssocsDialog(wxWindow* parent);
    protected:
        void OnApply(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void OnClearAll(wxCommandEvent& event);
    private:
        DECLARE_EVENT_TABLE()
};

#define ASC_ASSOC_DLG_NO_DONT_ASK       0
#define ASC_ASSOC_DLG_NO_ONLY_NOW       1
#define ASC_ASSOC_DLG_YES_CORE_FILES    2
#define ASC_ASSOC_DLG_YES_ALL_FILES     3

class AskAssocDialog : public wxScrollingDialog
{
    public:
        AskAssocDialog(wxWindow* parent);
    protected:
        void OnOK(wxCommandEvent& event);
        void OnESC(wxCommandEvent& event);
        void OnCharHook(wxKeyEvent& event);
    private:
        DECLARE_EVENT_TABLE()
};



#endif // ASSOCIATIONS_H
