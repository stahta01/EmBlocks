/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * Copyright: 2008 Jens Lody
 *
 * $Revision: 4 $
 * $Id: IncrementalSearchConfDlg.h 4 2013-11-02 15:53:52Z gerard $
 * $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/contrib/IncrementalSearch/IncrementalSearchConfDlg.h $
 */

#ifndef INCREMENTALSEARCHCONFDLG_H
#define INCREMENTALSEARCHCONFDLG_H

#include <configurationpanel.h>

class IncrementalSearchConfDlg : public cbConfigurationPanel
{
public:
    IncrementalSearchConfDlg(wxWindow* parent);
    ~IncrementalSearchConfDlg();

private:
    wxString GetTitle() const { return _("Incremental search"); }
    virtual wxBitmap GetBitmap() { return wxXmlResource::Get()->LoadBitmap(_T("bmp_incsearch"));}
    void OnApply() { SaveSettings(); }
    void OnCancel() {}
    void OnChooseColour(wxCommandEvent& event);
    void SaveSettings();

    DECLARE_EVENT_TABLE()
};

#endif // INCREMENTALSEARCHCONFDLG_H
