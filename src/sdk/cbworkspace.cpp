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

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "cbworkspace.h"
    #include "globals.h"
    #include "manager.h"
    #include "configmanager.h"
    #include "logmanager.h"
    #include "workspaceloader.h"

    #include <wx/intl.h>
#endif

#include <wx/filedlg.h>
#include "filefilters.h"

cbWorkspace::cbWorkspace(const wxString& filename) :
    m_IsOK(true),
    m_IsDefault(true),
    m_Modified(false),
    m_Filename(DEFAULT_WORKSPACE),
    m_Title(_("Default workspace"))
{
    //ctor
    if ( filename.Matches(DEFAULT_WORKSPACE) || filename.IsEmpty() )
    {
        // if no filename given, use the default workspace
        wxString tmp = ConfigManager::GetConfigFolder() + wxFILE_SEP_PATH;

        if (!wxDirExists(tmp))
            wxMkdir(tmp, 0755);

        tmp << wxFILE_SEP_PATH << DEFAULT_WORKSPACE;
        m_Filename = tmp;
    }
    else
    {
        m_Filename = filename;
        m_IsDefault = false;
    }

    if ( !filename.IsEmpty() )
    {
        Load();
    }
}

cbWorkspace::~cbWorkspace()
{
}

void cbWorkspace::Load()
{
    wxString fname = m_Filename.GetFullPath();
    #if wxCHECK_VERSION(2, 9, 0)
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Loading workspace \"%s\""), fname.wx_str()));
    #else
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Loading workspace \"%s\""), fname.c_str()));
    #endif

    if (!m_Filename.FileExists())
    {
        Manager::Get()->GetLogManager()->DebugLog(_T("File does not exist."));
        if (!m_IsDefault)
        {
            wxString msg;
            msg.Printf(_("Workspace '%s' does not exist..."), fname.c_str());
            cbMessageBox(msg, _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
            // workspace wasn't loaded succesfully
            m_IsOK = false;
            return;
        }
    }

    if (FileTypeOf(fname) == ftCodeBlocksWorkspace)
    {
        IBaseWorkspaceLoader* pWsp = new WorkspaceLoader;

        wxString Title;
        m_IsOK = pWsp && (pWsp->Open(fname, Title) || m_IsDefault);
        if(!Title.IsEmpty())
        {
            m_Title = Title;
        }

        delete pWsp;
    }

    m_Filename.SetExt(FileFilters::WORKSPACE_EXT);
    SetModified(false);
}

bool cbWorkspace::Save(bool force)
{
    if (m_Filename.GetFullPath().IsEmpty())
        return SaveAs(_T(""));

    if (!force && !m_Modified)
        return true;

    #if wxCHECK_VERSION(2, 9, 0)
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Saving workspace \"%s\""), m_Filename.GetFullPath().wx_str()));
    #else
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Saving workspace \"%s\""), m_Filename.GetFullPath().c_str()));
    #endif
    WorkspaceLoader wsp;
    bool ret = wsp.Save(m_Title, m_Filename.GetFullPath());
    SetModified(!ret);
    if(!ret)
        cbMessageBox(_("Couldn't save workspace ") + m_Filename.GetFullPath() + _("\n(Maybe the file is write-protected?)"), _("Warning"), wxICON_WARNING);
    return ret;
}

bool cbWorkspace::SaveAs(const wxString& filename)
{
    //Do we have a filename?
    if(!filename.IsEmpty())
    {
        wxFileName _Filename = filename;
        if (_Filename.GetExt() == wxEmptyString)
            _Filename.SetExt(FileFilters::WORKSPACE_EXT);

        if( _Filename.FileExists() )
        {
            if (cbMessageBox(_("Workspace file already exists.\nAre you really sure you want to OVERWRITE it?"),
                         _("Confirmation"), wxYES_NO | wxICON_QUESTION) == wxID_YES)
            {
                if (!wxRemoveFile(filename))
                {
                    cbMessageBox(_("Couldn't remove the old workspace file to create the new one.\nThe file might be read-only?!"),
                             _("Error"), wxICON_WARNING);
                    return false;
                }
            }
            else
                return false;
        }
        m_Filename = _Filename;
        m_IsDefault = false;
        return Save(true);
    }

    // We don't have a filename so ask for the name to save.
    wxFileDialog dlg(Manager::Get()->GetAppWindow(),
                     _("Save workspace"),
                     m_Filename.GetPath(),
                     m_Filename.GetFullName(),
                     FileFilters::GetFilterString(_T('.') + FileFilters::WORKSPACE_EXT),
                     wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() != wxID_OK)
        return false;

    m_Filename = dlg.GetPath();
    if (m_Filename.GetExt() == wxEmptyString)
        m_Filename.SetExt(FileFilters::WORKSPACE_EXT);

    if (m_Filename.GetFullName().Matches(DEFAULT_WORKSPACE))
        m_IsDefault = true;
    else
        m_IsDefault = false;

    return Save(true);
}

void cbWorkspace::SetTitle(const wxString& title)
{
    m_Title = title;
    SetModified(true);
}

void cbWorkspace::SetModified(bool modified)
{
    m_Modified = modified;
    // Manager::Get()->GetLogManager()->DebugLog(F(_T("Setting workspace to modified = \"%s\""), modified ? _T("true") : _T("false")));
}
