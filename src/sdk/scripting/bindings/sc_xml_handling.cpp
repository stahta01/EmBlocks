/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 101 $
 * $Id: sc_xml_handling.cpp 101 2014-01-23 14:39:42Z gerard $
 * $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/sdk/scripting/bindings/sc_progress.cpp $
 */

#include <sdk_precomp.h>

#ifndef CB_PRECOMP
#include <globals.h>
#include <cbexception.h>
#include <wx/string.h>
#endif

#include "sc_base_types.h"
#include <wx/zipstrm.h>
#include "tinyxml/tinywxuni.h"


class xmlFile
{
public:
    xmlFile()
    {
        m_pWorkingElement = NULL;
    }

    ~xmlFile()
    {
    }

    bool Open(wxString& _src);
    bool CWElement(wxString& element);

    void CWElementToRoot()
    {
        m_pWorkingElement = (TiXmlElement*)&m_doc;
    }

    bool CWElementToParent();
    wxString GetAttribute(wxString& attrib);
    wxString GetText();
    int GetChildList(wxArrayString& arrStr);

private:
    TiXmlDocument m_doc;
    TiXmlElement* m_pWorkingElement;

};


bool xmlFile::Open(wxString& _src)
{
    if( _src.Contains(_T("#zip:")))
    {
        wxString contents;
        wxFileSystem* fs = new wxFileSystem;
        wxFSFile* f = fs->OpenFile(_src);

        if (!f)
        {
            delete fs;
            return false;
        }

        {
            wxInputStream* is = f->GetStream();
            while (!is->Eof() && is->CanRead())
            {
                char tmp[1024];
                memset(tmp, 0, sizeof(tmp));
                is->Read(tmp, sizeof(tmp) - 1);
                contents << cbC2U((const char*)tmp);
            }
        }

        delete f;
        delete fs;

        m_doc.Parse(cbU2C(contents));
        m_pWorkingElement = m_doc.RootElement();
        return true;
    }
    else
    {
        if(TinyXML::LoadDocument(_src, &m_doc))
        {
            m_pWorkingElement =  (TiXmlElement*)&m_doc;
            return true;
        }
    }

    return false;
}


bool xmlFile::CWElement(wxString& filename)
{
    if(!m_pWorkingElement)
        return false;

    TiXmlElement* curElement = m_pWorkingElement;

    wxArrayString elements =  GetArrayFromString(filename, _T('/'));

    for(size_t i=0; i < elements.GetCount(); i++)
    {
        if(elements[i] == _T("."))
            continue;

        if(elements[i] == _T(".."))
        {
            if(!CWElementToParent())
            {
                m_pWorkingElement = curElement;
                return false;
            }

        }
        else
        {
            TiXmlElement* tmp = m_pWorkingElement->FirstChildElement(cbU2C(elements[i]));
            if(tmp)
            {
                m_pWorkingElement = tmp;
            }
            else
            {
                m_pWorkingElement = curElement;
                return false;
            }
        }
    }

    return ( curElement == m_pWorkingElement ? false :true);
}

bool xmlFile::CWElementToParent()
{
    if(!m_pWorkingElement)
        return false;

    if( m_pWorkingElement->Parent() )
    {
        m_pWorkingElement = (TiXmlElement*)m_pWorkingElement->Parent();
        return true;
    }

    return false;
}

wxString xmlFile::GetAttribute(wxString& attrib)
{
    wxString ret = _T("");

    if(m_pWorkingElement)
        ret = cbC2U(m_pWorkingElement->Attribute(cbU2C(attrib)));

    ret.Replace(_T("\""), _T(""));
    return ret;
}

wxString xmlFile::GetText()
{
    wxString ret = _T("");

    if(m_pWorkingElement)
        ret = cbC2U(m_pWorkingElement->GetText());
    return ret;
}

int xmlFile::GetChildList(wxArrayString& arrStr)
{
    arrStr.Clear();

    if(!m_pWorkingElement)
        return -1;

    TiXmlElement* elem = (TiXmlElement*)m_pWorkingElement->FirstChild();
    while(elem)
    {
        wxString name = cbC2U(elem->Value());
        if(!name.IsEmpty())  //Root??
        {
            if(elem->Attribute("alt_title"))
            {
                name = cbC2U(elem->Attribute("alt_title"));
                name.Replace(_T("\""), _T(""));
            }

            if(!name.IsEmpty())
                arrStr.Add(name);
        }
        elem = (TiXmlElement*)elem->NextSibling();
    }

    return arrStr.GetCount();
}

namespace ScriptBindings
{
void Register_XmlHandling()
{
    SqPlus::SQClassDef<xmlFile>("xmlFile").
    emptyCtor().
    func(&xmlFile::Open, "Open").
    func(&xmlFile::CWElement, "CWE").
    func(&xmlFile::CWElementToRoot, "CWEtoRoot").
    func(&xmlFile::CWElementToParent, "CWEtoParent").
    func(&xmlFile::GetAttribute, "GetAttribute").
    func(&xmlFile::GetText, "GetText").
    func(&xmlFile::GetChildList, "GetChildList");
}
} // namespace ScriptBindings
