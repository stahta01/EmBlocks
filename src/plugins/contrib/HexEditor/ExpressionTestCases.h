/*
* This file is part of HexEditor plugin for Em::Blocks Studio
* Copyright (C) 2008-2009 Bartlomiej Swiecki
*
* HexEditor plugin is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* HexEditor pluging is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with HexEditor. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 4 $
* $Id: ExpressionTestCases.h 4 2013-11-02 15:53:52Z gerard $
* $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/contrib/HexEditor/ExpressionTestCases.h $
*/

#ifndef EXPRESSIONTESTCASES_H
#define EXPRESSIONTESTCASES_H

#include <wx/string.h>

#include "TestCasesBase.h"

namespace Expression
{
    TestCasesBase& GetTests();
}

#endif