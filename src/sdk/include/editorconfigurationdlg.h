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

#ifndef EDITORCONFIGURATION_H
#define EDITORCONFIGURATION_H

#include "scrollingdialog.h"
#include <wx/arrstr.h>
#include "settings.h"

// forward decls
class cbStyledTextCtrl;
class EditorColourSet;
class wxListbookEvent;

class EditorConfigurationDlg : public wxScrollingDialog
{
    public:
        EditorConfigurationDlg(wxWindow* parent);
        ~EditorConfigurationDlg();

        void OnChooseFont(wxCommandEvent& event);
        void OnChooseColour(wxCommandEvent& event);
        void OnBoldItalicUline(wxCommandEvent& event);
        void OnColourTheme(wxCommandEvent& event);
        void OnAddColourTheme(wxCommandEvent& event);
        void OnDeleteColourTheme(wxCommandEvent& event);
        void OnRenameColourTheme(wxCommandEvent& event);
        void OnColourComponent(wxCommandEvent& event);
        void OnColoursReset(wxCommandEvent& event);
        void OnEditKeywords(wxCommandEvent& event);
        void OnEditFilemasks(wxCommandEvent& event);
        void OnChangeLang(wxCommandEvent& event);
        void OnChangeDefCodeFileType(wxCommandEvent& event);
        void OnDynamicCheck(wxCommandEvent& event);
        void OnHighlightOccurrences(wxCommandEvent& event);
        void OnMultipleSelections(wxCommandEvent& event);
        void UpdateSampleFont(bool askForNewFont);
        void OnCaretStyle(wxCommandEvent& event);

        void EndModal(int retCode);
    private:
        void OnPageChanged(wxListbookEvent& event);
        void AddPluginPanels();
        void UpdateListbookImages();
        void CreateColoursSample();
        void ApplyColours();
        void FillColourComponents();
        void ReadColours();
        void WriteColours();
        void LoadThemes();
        void ChangeTheme();
        bool AskToSaveTheme();
        cbStyledTextCtrl* m_TextColourControl;
        EditorColourSet* m_Theme;
        HighlightLanguage m_Lang;
        int m_DefCodeFileType;
        wxArrayString m_DefaultCode;  //!< will store the default code during configuration for each type
        bool m_ThemeModified;
        ConfigurationPanelsArray m_PluginPanels;
        wxString m_FontString; // This is to store font data temporarily
        bool m_EnableChangebar; // remember whether changebar was enabled or not
        bool m_EnableScrollWidthTracking; // remember whether scrollbarwidthtracking was enabled or not

        DECLARE_EVENT_TABLE()
};

#endif // EDITORCONFIGURATION_H
