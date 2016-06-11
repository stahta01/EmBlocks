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
#include "compilervaroptions.h"

CompilerVarOptions::CompilerVarOptions()
{
}

CompilerVarOptions::CompilerVarOptions(const CompilerVarOptions& other)
{
    // copy ctor
    *this = other;
}

CompilerVarOptions& CompilerVarOptions::operator=(const CompilerVarOptions& other)
{
    ClearOptions();
	for (unsigned int i = 0; i < other.m_Options.GetCount(); ++i)
	{
        CompVarOption* coption = new CompVarOption(*(other.m_Options[i]));
        AddOption(coption);
	}
	return *this;
}

CompilerVarOptions::~CompilerVarOptions()
{
	ClearOptions();
}

void CompilerVarOptions::ClearOptions()
{
	for (unsigned int i = 0; i < m_Options.GetCount(); ++i)
	{
		CompVarOption* coption = m_Options.Item(i);
		delete coption;
	}
	m_Options.Clear();
}

void CompilerVarOptions::AddOption(CompVarOption* coption )
{
	m_Options.Add(coption);
}

void CompilerVarOptions::AddOption(  const wxString& name,
                                    const wxString& macro,
                                    const wxString& format,
                                    const wxString& toolTip,
                                    const wxString& value)
{
	if (name.IsEmpty() || macro.IsEmpty() || format.IsEmpty() )
		return;

	CompVarOption* coption = new CompVarOption;

	coption->name   = name;
	coption->macro  = macro;
	coption->format = format;
    coption->toolTip = toolTip;
    coption->value  = value;
	AddOption(coption);
}

CompVarOption* CompilerVarOptions::GetOptionByMacro(const wxString& macro)
{
	for (unsigned int i = 0; i < m_Options.GetCount(); ++i)
	{
		CompVarOption* coption = m_Options.Item(i);
		if (coption->macro == macro)
			return coption;
	}
	return 0L;
}

CompVarOption* CompilerVarOptions::GetOptionByName(const wxString& name)
{
	for (unsigned int i = 0; i < m_Options.GetCount(); ++i)
	{
		CompVarOption* coption = m_Options.Item(i);
		if (coption->name == name)
			return coption;
	}
	return 0L;
}

/*
CompVarOption* CompilerVarOptions::GetOptionByOption(const wxString& option)
{
	for (unsigned int i = 0; i < m_Options.GetCount(); ++i)
	{
		CompVarOption* coption = m_Options.Item(i);
		if (coption->option == option)
			return coption;
	}
	return 0L;
}
*/

