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
#include <scrollingdialog.h>
#include <manager.h>
#include <configmanager.h>
#include <pluginmanager.h>
#include <cbplugin.h>
#include <globals.h>

// Custom window to shutdown the app when closed.
// used for batch builds only.
class BatchLogWindow : public wxScrollingDialog
{
    public:
        BatchLogWindow(wxWindow *parent, const wxChar *title)
            : wxScrollingDialog(parent, -1, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)
        {
            wxSize size;
            size.SetWidth(Manager::Get()->GetConfigManager(_T("message_manager"))->ReadInt(_T("/batch_build_log/width"), wxDefaultSize.GetWidth()));
            size.SetHeight(Manager::Get()->GetConfigManager(_T("message_manager"))->ReadInt(_T("/batch_build_log/height"), wxDefaultSize.GetHeight()));
            SetSize(size);
        }
        void EndModal(int retCode)
        {
            // allowed to close?
            // find compiler plugin
            PluginsArray arr = Manager::Get()->GetPluginManager()->GetCompilerOffers();
            if (arr.GetCount() != 0)
            {
                cbCompilerPlugin* compiler = static_cast<cbCompilerPlugin*>(arr[0]);
                if (compiler && compiler->IsRunning())
                {
                    if (cbMessageBox(_("The build is in progress. Are you sure you want to abort it?"),
                                    _("Abort build?"),
                                    wxICON_QUESTION | wxYES_NO, this) == wxID_YES)
                    {
                        compiler->KillProcess();
                        while (compiler->IsRunning())
                        {
                            wxMilliSleep(100);
                            Manager::Yield();
                        }
                    }
                    return;
                }
            }

            Manager::Get()->GetConfigManager(_T("message_manager"))->Write(_T("/batch_build_log/width"), (int)GetSize().GetWidth());
            Manager::Get()->GetConfigManager(_T("message_manager"))->Write(_T("/batch_build_log/height"), (int)GetSize().GetHeight());
            wxScrollingDialog::EndModal(retCode);
        }
};
