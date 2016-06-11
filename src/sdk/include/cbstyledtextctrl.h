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

#ifndef CBSTYLEDTEXTCTRL_H_INCLUDED
#define CBSTYLEDTEXTCTRL_H_INCLUDED

#include "wx/wxscintilla.h"
#include <wx/longlong.h>

#include <map>
#include <set>

class wxContextMenuEvent;
class wxFocusEvent;
class wxMouseEvent;

class cbStyledTextCtrl : public wxScintilla
{
    public:
        cbStyledTextCtrl(wxWindow* pParent, int id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
        virtual ~cbStyledTextCtrl();
        wxDateTime GetLastFocusTime() const {return m_lastFocusTime;}

        void EnableTabSmartJump(bool enable = true);
        bool IsCharacter(int style);
        bool IsString(int style);
        bool IsPreprocessor(int style);
        bool IsComment(int style);
        void CallTipCancel();
        wxWindow* GetParent() {return m_pParent;}

        static std::map<int, std::set<int> > &GetCharacterLexerStyles();
        static std::map<int, std::set<int> > &GetStringLexerStyles();
        static std::map<int, std::set<int> > &GetPreprocessorLexerStyles();
        static std::map<int, std::set<int> > &GetCommentLexerStyles();

    private:
        void OnContextMenu(wxContextMenuEvent& event);
        void OnKillFocus(wxFocusEvent& event);
        void OnSetFocus(wxFocusEvent& event);
        void OnMouseMiddleDown(wxMouseEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void OnKeyUp(wxKeyEvent& event);
        void OnMouseLeftUp(wxMouseEvent& event);

        bool AllowTabSmartJump();
        void HighlightRightBrace();

        wxWindow* m_pParent;
        wxLongLong m_lastFocusTime;
        int m_bracePosition;
        int m_lastPosition;
        bool m_tabSmartJump;

        static std::map<int, std::set<int> > CharacterLexerStyles, StringLexerStyles, PreprocessorLexerStyles, CommentLexerStyles;

        DECLARE_EVENT_TABLE()
};

#endif // CBSTYLEDTEXTCTRL_H_INCLUDED
