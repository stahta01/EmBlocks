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

#ifndef PROJECTTEMPLATELOADER_H
#define PROJECTTEMPLATELOADER_H

#include "tinyxml/tinyxml.h"
#include "settings.h"

struct FileSetFile
{
	wxString source;
	wxString destination;
	wxString targets;
};
WX_DECLARE_OBJARRAY(FileSetFile, FileSetFileArray);

struct FileSet
{
	wxString name;
	wxString title;
	FileSetFileArray files;
};
WX_DECLARE_OBJARRAY(FileSet, FileSetArray);

struct TemplateOption
{
	wxString name;
	wxString notice;
	int noticeMsgType;
	bool useDefaultCompiler;
	wxString file;
	wxArrayString extraCFlags;
	wxArrayString extraLDFlags;
};
WX_DECLARE_OBJARRAY(TemplateOption, TemplateOptionArray);

class DLLIMPORT ProjectTemplateLoader
{
	public:
		ProjectTemplateLoader();
		virtual ~ProjectTemplateLoader();

        bool Open(const wxString& filename);

		wxString m_Name;
		wxString m_Notice;
		int m_NoticeMsgType;
		wxString m_Title;
		wxString m_Category;
		wxString m_Bitmap;
		FileSetArray m_FileSets;
		TemplateOptionArray m_TemplateOptions;
	protected:
        void DoTemplate(TiXmlElement* parentNode);
        void DoTemplateNotice(TiXmlElement* parentNode);
        void DoFileSet(TiXmlElement* parentNode);
        void DoFileSetFile(TiXmlElement* parentNode, FileSet& fs);
        void DoOption(TiXmlElement* parentNode);
        void DoOptionProject(TiXmlElement* parentNode, TemplateOption& to);
        void DoOptionCompiler(TiXmlElement* parentNode, TemplateOption& to);
        void DoOptionLinker(TiXmlElement* parentNode, TemplateOption& to);
	private:
};

#endif // PROJECTTEMPLATELOADER_H
