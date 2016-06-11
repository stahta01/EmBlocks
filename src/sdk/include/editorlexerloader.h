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

#ifndef EDITORLEXERLOADER_H
#define EDITORLEXERLOADER_H

#include <wx/string.h>

class EditorColourSet;
class LoaderBase;
class TiXmlElement;

class EditorLexerLoader
{
	public:
		EditorLexerLoader(EditorColourSet* target);
		virtual ~EditorLexerLoader();

		void Load(LoaderBase *loader);
	protected:
        void DoLexer(TiXmlElement* node);
        void DoStyles(HighlightLanguage language, TiXmlElement* node);
        void DoKeywords(HighlightLanguage language, TiXmlElement* node);
        void DoSingleKeywordNode(HighlightLanguage language, TiXmlElement* node, const wxString& nodename);
        void DoSampleCode(HighlightLanguage language, TiXmlElement* node);
        void DoLangAttributes(HighlightLanguage language, TiXmlElement* node);
        bool DoLangAttributesLexerStyles(TiXmlElement* attribs, const char *attributeName, std::set<int> &styles);
        EditorColourSet* m_pTarget;
	private:
};

#endif // EDITORLEXERLOADER_H
