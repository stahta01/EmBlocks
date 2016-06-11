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
#ifndef SCRIPTINGSETTINGSDLG_H
#define SCRIPTINGSETTINGSDLG_H

#include <vector>
#include <wx/listctrl.h> // wxListEvent

#include "scrollingdialog.h"

class ScriptEntry// : public ISerializable (<-- can't make it work)
{
public:
    wxString SerializeOut() const
    {
        return wxString::Format(_T("%s;%d;%d;%s"), script.c_str(), enabled ? 1 : 0, registered ? 1 : 0, menu.c_str());
    }
    void SerializeIn(const wxString& s)
    {
        wxString tmp = s;
        script = tmp.BeforeFirst(_T(';'));
        tmp = tmp.AfterFirst(_T(';'));
        enabled = tmp.BeforeFirst(_T(';')) == _T("1") ? true : false;
        tmp = tmp.AfterFirst(_T(';'));
        registered = tmp.BeforeFirst(_T(';')) == _T("1") ? true : false;
        tmp = tmp.AfterFirst(_T(';'));
        menu = tmp.BeforeFirst(_T(';'));
    }

    wxString script;
    bool enabled;
    bool registered;
    wxString menu;
};
typedef std::vector<ScriptEntry> ScriptsVector;

class ScriptingSettingsDlg : public wxScrollingDialog
{
    public:
        ScriptingSettingsDlg(wxWindow* parent);
        ~ScriptingSettingsDlg();

        void EndModal(int retCode);
    protected:
        void FillScripts();
        void UpdateState();
        void FillTrusts();
        void UpdateTrustsState();
        void LoadItem(long item);
        void SaveItem(long item);

        void OnListSelection(wxListEvent& event);
        void OnListDeselection(wxListEvent& event);
        void OnAddScript(wxCommandEvent& event);
        void OnRemoveScript(wxCommandEvent& event);
        void OnScriptChanged(wxCommandEvent& event);
        void OnScriptMenuChanged(wxCommandEvent& event);
        void OnEnable(wxCommandEvent& event);
        void OnRegister(wxCommandEvent& event);
        void OnBrowse(wxCommandEvent& event);

        void OnTrustSelection(wxListEvent& event);
        void OnDeleteTrust(wxCommandEvent& event);
        void OnValidateTrusts(wxCommandEvent& event);

        ScriptsVector m_ScriptsVector;
        bool m_IgnoreTextEvents;
    private:
        DECLARE_EVENT_TABLE()
};



#endif // SCRIPTINGSETTINGSDLG_H
