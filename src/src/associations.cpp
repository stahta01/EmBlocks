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

#include <sdk.h>
#ifndef CB_PRECOMP
    #include <wx/radiobox.h>
    #include <wx/xrc/xmlres.h>
#endif
#include "associations.h"
#include "appglobals.h"
#include <manager.h>
#include <configmanager.h>
#include <filefilters.h>
#include <wx/checklst.h>


#define NUMBER_OF_ASSOC (int)(sizeof(knownTypes)/sizeof(knownTypes[0]))
const Associations::Assoc knownTypes[] =
{
    PROGRAM_FILE_ASSOCIATIONS()
};


inline void DoSetAssociation(const wxString& executable, int index)
{
    Associations::DoSetAssociation(knownTypes[index].ext, knownTypes[index].descr, executable, knownTypes[index].index);
};

inline bool DoCheckAssociation(const wxString& executable, int index)
{
    return Associations::DoCheckAssociation(knownTypes[index].ext, knownTypes[index].descr, executable, knownTypes[index].index);
};

void Associations::SetBatchBuildOnly()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    ::DoSetAssociation(name, 0);
    ::DoSetAssociation(name, 1);

    UpdateChanges();
}

void Associations::UpdateChanges()
{
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0L, 0L);
}

void Associations::SetCore()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for (int i = 0; i < NUMBER_OF_CORE_ASSOC; ++i)
        ::DoSetAssociation(name, i);

    UpdateChanges();
}

void Associations::SetAll()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for (unsigned int i = 0; i < NUMBER_OF_ASSOC; ++i)
        ::DoSetAssociation(name, i);

    UpdateChanges();
}

void Associations::ClearAll()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for (unsigned int i = 0; i < NUMBER_OF_ASSOC; ++i)
        DoClearAssociation(knownTypes[i].ext);

    UpdateChanges();
}

bool Associations::Check()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    bool result = true;

    for (int i = 0; i < NUMBER_OF_ASSOC; ++i)
        result &= ::DoCheckAssociation(name, i);

    return  result;
}

void Associations::DoSetAssociation(const wxString& ext, const wxString& descr, const wxString& exe, int icoNum)
{
    wxString BaseKeyName(_T("HKEY_CURRENT_USER\\Software\\Classes\\"));
    if (platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = _T("HKEY_CLASSES_ROOT\\");

    wxString node(_T(KEY_NAME) + ext);

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + _T(".") + ext);
    key.Create();
    key = _T(KEY_NAME) + ext;

    key.SetName(BaseKeyName + node);
    key.Create();
    key = descr;

    key.SetName(BaseKeyName + node + _T("\\DefaultIcon"));
    key.Create();
    key = exe + wxString::Format(_T(",%d"), icoNum);

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\command"));
    key.Create();
    key = _T("\"") + exe + _T("\" \"%1\"");

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\ddeexec"));
    key.Create();
    key = _T("[Open(\"%1\")]");

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\ddeexec\\application"));
    key.Create();
    key = DDE_SERVICE;

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\ddeexec\\ifexec"));
    key.Create();
    key = _T("[IfExec_Open(\"%1\")]");;

    key.SetName(BaseKeyName + node + _T("\\shell\\open\\ddeexec\\topic"));
    key.Create();
    key = DDE_TOPIC;

    if (ext.IsSameAs(FileFilters::EMBLOCKS_EXT) || ext.IsSameAs(FileFilters::WORKSPACE_EXT))
    {
        wxString batchbuildargs = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/batch_build_args"), appglobals::DefaultBatchBuildArgs);
        key.SetName(BaseKeyName + node + _T("\\shell\\Build\\command"));
        key.Create();
        key = _T("\"") + exe + _T("\" ") + batchbuildargs + _T(" --build \"%1\"");

        key.SetName(BaseKeyName + node + _T("\\shell\\Rebuild (clean)\\command"));
        key.Create();
        key = _T("\"") + exe + _T("\" ") + batchbuildargs + _T(" --rebuild \"%1\"");
    }
}

void Associations::DoClearAssociation(const wxString& ext)
{
    wxString BaseKeyName(_T("HKEY_CURRENT_USER\\Software\\Classes\\"));
    if (platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = _T("HKEY_CLASSES_ROOT\\");

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + _T(".") + ext);
    if (key.Exists())
    {
        wxString s;
        #if wxCHECK_VERSION(2, 9, 0)
        if (key.QueryValue(wxEmptyString, s) && s.StartsWith(_T(APP_NAME)))
        #else
        if (key.QueryValue(NULL, s) && s.StartsWith(_T(APP_NAME)))
        #endif
            key.DeleteSelf();
    }

    key.SetName(BaseKeyName + _T(KEY_NAME) + ext);
    if (key.Exists())
        key.DeleteSelf();
}

bool Associations::DoCheckAssociation(const wxString& ext, const wxString& descr, const wxString& exe, int icoNum)
{
    wxString BaseKeyName(_T("HKEY_CURRENT_USER\\Software\\Classes\\"));

    if (platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = _T("HKEY_CLASSES_ROOT\\");

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + _T(".") + ext);
    if (!key.Exists())
        return false;

    key.SetName(BaseKeyName + _T(KEY_NAME) + ext);
    if (!key.Exists())
        return false;

    key.SetName(BaseKeyName + _T(KEY_NAME)+ ext + _T("\\DefaultIcon"));
    if (!key.Exists())
        return false;
    wxString strVal;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != wxString::Format(_T("%s,%d"), exe.c_str(), icoNum))
        return false;

    key.SetName(BaseKeyName + _T(KEY_NAME) + ext + _T("\\shell\\open\\command"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != wxString::Format(_T("\"%s\" \"%%1\""), exe.c_str()))
        return false;

    key.SetName(BaseKeyName + _T(KEY_NAME) + ext + _T("\\shell\\open\\ddeexec"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != _T("[Open(\"%1\")]"))
        return false;

    key.SetName(BaseKeyName + _T(KEY_NAME) + ext + _T("\\shell\\open\\ddeexec\\application"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != DDE_SERVICE)
        return false;

    key.SetName(BaseKeyName + _T(KEY_NAME) + ext + _T("\\shell\\open\\ddeexec\\ifexec"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != _T("[IfExec_Open(\"%1\")]"))
        return false;

    key.SetName(BaseKeyName + _T(KEY_NAME) + ext + _T("\\shell\\open\\ddeexec\\topic"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != DDE_TOPIC)
        return false;

    if (ext.IsSameAs(FileFilters::EMBLOCKS_EXT) || ext.IsSameAs(FileFilters::WORKSPACE_EXT))
    {
        wxString batchbuildargs = Manager::Get()->GetConfigManager(_T("app"))->Read(_T("/batch_build_args"), appglobals::DefaultBatchBuildArgs);
        key.SetName(BaseKeyName + _T(KEY_NAME) + ext + _T("\\shell\\Build\\command"));
        if (!key.Open())
            return false;
        if (!key.QueryValue(wxEmptyString, strVal))
            return false;
        if (strVal != _T("\"") + exe + _T("\" ") + batchbuildargs + _T(" --build \"%1\""))
            return false;

        key.SetName(BaseKeyName + _T(KEY_NAME) + ext + _T("\\shell\\Rebuild (clean)\\command"));
        if (!key.Open())
            return false;
        if (!key.QueryValue(wxEmptyString, strVal))
            return false;
        if (strVal != _T("\"") + exe + _T("\" ") + batchbuildargs + _T(" --rebuild \"%1\""))
            return false;
    }

    return true;
}



BEGIN_EVENT_TABLE(ManageAssocsDialog, wxScrollingDialog)
    EVT_BUTTON(XRCID("wxID_OK"), ManageAssocsDialog::OnApply)
    EVT_BUTTON(XRCID("wxID_CANCEL"), ManageAssocsDialog::OnCancel)
    EVT_BUTTON(XRCID("clearAll"), ManageAssocsDialog::OnClearAll)
END_EVENT_TABLE()



ManageAssocsDialog::ManageAssocsDialog(wxWindow* parent)
{
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgManageAssocs"),_T("wxScrollingDialog"));

    list = XRCCTRL(*this, "checkList", wxCheckListBox);
    assert(list);

    wxString d(_T("."));

    wxChar exe[MAX_PATH] = {0};
    GetModuleFileName(0L, exe, MAX_PATH);

    for (unsigned int i = 0; i < NUMBER_OF_ASSOC; ++i)
    {
        list->Append(d + knownTypes[i].ext + _T("  (") + knownTypes[i].descr + _T(")"));
        list->Check(i, Associations::DoCheckAssociation(knownTypes[i].ext, knownTypes[i].descr, exe, knownTypes[i].index));
    }

    CentreOnParent();
}

void ManageAssocsDialog::OnApply(wxCommandEvent& event)
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for (int i = 0; i < (int)list->GetCount(); ++i)
    {
        if (list->IsChecked(i))
            ::DoSetAssociation(name, i);
        else
            Associations::DoClearAssociation(knownTypes[i].ext);
    }

    Associations::UpdateChanges();
    EndModal(0);
}

void ManageAssocsDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}

void ManageAssocsDialog::OnClearAll(wxCommandEvent& event)
{
    Associations::ClearAll();
    Associations::UpdateChanges();
    EndModal(0);
}

BEGIN_EVENT_TABLE(AskAssocDialog, wxScrollingDialog)
    EVT_BUTTON(XRCID("wxID_OK"), AskAssocDialog::OnOK)
    EVT_BUTTON(wxID_CANCEL, AskAssocDialog::OnESC)
    EVT_CHAR_HOOK(AskAssocDialog::OnCharHook)
END_EVENT_TABLE()

AskAssocDialog::AskAssocDialog(wxWindow* parent)
{
    wxXmlResource::Get()->LoadObject(this, parent, _T("askAssoc"),_T("wxScrollingDialog"));
    SetEscapeId(wxID_NONE);
}

void AskAssocDialog::OnOK(wxCommandEvent& event)
{
    EndModal(XRCCTRL(*this, "choice", wxRadioBox)->GetSelection());
}

void AskAssocDialog::OnESC(wxCommandEvent& event)
{
    EndModal(ASC_ASSOC_DLG_NO_ONLY_NOW);
}

void AskAssocDialog::OnCharHook(wxKeyEvent& event)
{
    if ( event.GetKeyCode() == WXK_ESCAPE )
        Close(); //wxDialog::Close() send button event with id wxID_CANCEL (wxWidgets 2.8)
    else if ( event.GetKeyCode() == WXK_RETURN )
        EndModal(XRCCTRL(*this, "choice", wxRadioBox)->GetSelection());

    event.Skip();
}
