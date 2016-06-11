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
#include "compileroptions.h"

CompilerOptions::CompilerOptions()
{
}

CompilerOptions::CompilerOptions(const CompilerOptions& other)
{
    // copy ctor
    *this = other;
}

CompilerOptions& CompilerOptions::operator=(const CompilerOptions& other)
{
    ClearOptions();
	for (unsigned int i = 0; i < other.m_Options.GetCount(); ++i)
	{
        CompOption* coption = new CompOption(*(other.m_Options[i]));
        AddOption(coption);
	}
	return *this;
}

CompilerOptions::~CompilerOptions()
{
	ClearOptions();
}

void CompilerOptions::ClearOptions()
{
	for (unsigned int i = 0; i < m_Options.GetCount(); ++i)
	{
		CompOption* coption = m_Options.Item(i);
		delete coption;
	}
	m_Options.Clear();
}

void CompilerOptions::AddOption(CompOption* coption )
{
	m_Options.Add(coption);
}

void CompilerOptions::AddOption(const wxString& name,
								const wxString& option,
								const wxString& category,
								//const wxString& additionalLibs,
								unsigned int optGroup,
								bool doChecks,
								const wxString& checkAgainst,
								const wxString& checkMessage)
{
	if (name.IsEmpty() || option.IsEmpty() )
		return;
	CompOption* coption = new CompOption;

	wxString listboxname = name; /* + _T("  [");
	if (option.IsEmpty())
        listboxname += additionalLibs;
    else
        listboxname += option;
    listboxname += _T("]"); */

	coption->name = listboxname;
	coption->option = option;
	coption->enabled = false;
	coption->category = category;
	coption->doChecks = doChecks;
	coption->checkAgainst = checkAgainst;
	coption->checkMessage = checkMessage;
	coption->group = optGroup;
	AddOption(coption);
}

CompOption* CompilerOptions::GetOptionByName(const wxString& name)
{
	for (unsigned int i = 0; i < m_Options.GetCount(); ++i)
	{
		CompOption* coption = m_Options.Item(i);
		if (coption->name == name)
			return coption;
	}
	return 0L;
}

CompOption* CompilerOptions::GetOptionByOption(const wxString& option)
{
	for (unsigned int i = 0; i < m_Options.GetCount(); ++i)
	{
		CompOption* coption = m_Options.Item(i);
		if (coption->option == option)
			return coption;
	}
	return 0L;
}


