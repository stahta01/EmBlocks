/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 4 $
 * $Id: asktypedlg.cpp 4 2013-11-02 15:53:52Z gerard $
 * $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/todo/asktypedlg.cpp $
 */

#include "sdk.h"
#ifndef CB_PRECOMP
  #include <wx/choice.h>
  #include <wx/intl.h>
  #include <wx/string.h>
  #include <wx/xrc/xmlres.h>
#endif
#include "asktypedlg.h"


AskTypeDlg::AskTypeDlg(wxWindow* parent)
{
    wxXmlResource::Get()->LoadObject(this, parent, _T("dlgAskType"),_T("wxScrollingDialog"));
}

AskTypeDlg::~AskTypeDlg()
{
    //dtor
}

TypeCorrection AskTypeDlg::GetTypeCorrection() const
{
    return static_cast<TypeCorrection>(XRCCTRL(*this, "chcCorrect", wxChoice)->GetSelection());
}
