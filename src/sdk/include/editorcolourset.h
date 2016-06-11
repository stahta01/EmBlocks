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

#ifndef EDITORCOLORSET_H
#define EDITORCOLORSET_H

#include <wx/dynarray.h>
#include <wx/hashmap.h>
#include <wx/intl.h>
#include <wx/wxscintilla.h> // wxSCI_KEYWORDSET_MAX
#include "settings.h"
#include "globals.h" // HighlightLanguage

#include <set>

// forward decls
class cbEditor;
class cbStyledTextCtrl;

#define COLORSET_DEFAULT	_T("default")

struct OptionColour
{
	wxString name;
	int value;
	wxColour fore;
	wxColour back;
	bool bold;
	bool italics;
	bool underlined;
	bool isStyle;

	wxColour originalfore;
	wxColour originalback;
	bool originalbold;
	bool originalitalics;
	bool originalunderlined;
	bool originalisStyle;
};
WX_DEFINE_ARRAY(OptionColour*, OptionColours);

struct CommentToken {
    wxString lineComment;
    wxString streamCommentStart;
    wxString streamCommentEnd;
    wxString boxCommentStart;
    wxString boxCommentMid;
    wxString boxCommentEnd;
};

struct OptionSet
{
    wxString m_Langs;
    OptionColours m_Colours;
    wxString m_Keywords[wxSCI_KEYWORDSET_MAX + 1]; // wxSCI_KEYWORDSET_MAX+1 keyword sets
    wxArrayString m_FileMasks;
    int m_Lexers;
    wxString m_SampleCode;
    int m_BreakLine;
    int m_DebugLine;
    int m_ErrorLine;

    wxString m_originalKeywords[wxSCI_KEYWORDSET_MAX + 1]; // wxSCI_KEYWORDSET_MAX+1 keyword sets
    wxArrayString m_originalFileMasks;

    CommentToken comment;
    bool m_CaseSensitive;
    bool m_Internal;
};
WX_DECLARE_STRING_HASH_MAP(OptionSet, OptionSetsMap);

class EditorColourSet
{
	public:
		EditorColourSet(const wxString& setName = COLORSET_DEFAULT);
		EditorColourSet(const EditorColourSet& other); // copy ctor
		~EditorColourSet();

		HighlightLanguage AddHighlightLanguage(int lexer, const wxString& name);
		HighlightLanguage GetHighlightLanguage(int lexer); // from scintilla lexer (wxSCI_LEX_*)
		HighlightLanguage GetHighlightLanguage(const wxString& name);
		wxArrayString GetAllHighlightLanguages();

		void AddOption(HighlightLanguage lang,
						const wxString& name,
						int value,
						wxColour fore = wxNullColour,
						wxColour back = wxNullColour,
						bool bold = false,
						bool italics = false,
						bool underlined = false,
						bool isStyle = true);
		bool AddOption(HighlightLanguage lang, OptionColour* option, bool checkIfExists = true);
		OptionColour* GetOptionByName(HighlightLanguage lang, const wxString& name);
		OptionColour* GetOptionByValue(HighlightLanguage lang, int value);
		OptionColour* GetOptionByIndex(HighlightLanguage lang, int index);
		void UpdateOptionsWithSameName(HighlightLanguage lang, OptionColour* base);
		int GetOptionCount(HighlightLanguage lang);
		HighlightLanguage GetLanguageForFilename(const wxString& filename);
		wxString GetLanguageName(HighlightLanguage lang);
		wxString GetName(){ return m_Name; }
		void SetName(const wxString& name){ m_Name = name; }
		HighlightLanguage Apply(cbEditor* editor, HighlightLanguage lang=HL_AUTO);
		void Apply(HighlightLanguage lang, cbStyledTextCtrl* control);
		void Save();
		void Reset(HighlightLanguage lang);
		wxString& GetKeywords(HighlightLanguage lang, int idx);
		void SetKeywords(HighlightLanguage lang, int idx, const wxString& keywords);
		const wxArrayString& GetFileMasks(HighlightLanguage lang);
		void SetFileMasks(HighlightLanguage lang, const wxString& masks, const wxString& show = _T("false"), const wxString& = _T(","));
		void SetInternalMark(HighlightLanguage lang, bool internal);
		bool GetInternalMark(HighlightLanguage lang);
		wxString GetSampleCode(HighlightLanguage lang, int* breakLine, int* debugLine, int* errorLine);
		void SetSampleCode(HighlightLanguage lang, const wxString& sample, int breakLine, int debugLine, int errorLine);
        CommentToken GetCommentToken(HighlightLanguage lang);
        void SetCommentToken(HighlightLanguage lang, CommentToken token);
        bool GetCaseSensitivity(HighlightLanguage lang);
        void SetCaseSensitivity(HighlightLanguage lang, bool CaseSensitive);
        void SetStringLexerStyles(HighlightLanguage lang, const std::set<int> &styles);
        void SetCommentLexerStyles(HighlightLanguage lang, const std::set<int> &styles);
        void SetCharacterLexerStyles(HighlightLanguage lang, const std::set<int> &styles);
        void SetPreprocessorLexerStyles(HighlightLanguage lang, const std::set<int> &styles);
	protected:
	private:
		void DoApplyStyle(cbStyledTextCtrl* control, int value, OptionColour* option);
		void LoadAvailableSets();
		void Load();
		void ClearAllOptionColours();

		wxString m_Name;
		OptionSetsMap m_Sets;
};

#endif // EDITORCOLORSET_H
