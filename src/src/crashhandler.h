/*
 * This file is part of the code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
*/
/*
    Copyright (C) Em::Blocks 2011-2013

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


    @version $Revision: 4 $:
    @author  $Author: gerard $:
    @date    $Date: 2013-11-02 16:53:52 +0100 (Sat, 02 Nov 2013) $:

 */
#ifndef CRASH_HANDLER
#define CRASH_HANDLER

#if (__WXMSW__)

#include <winnt.h>

class CrashHandler
{
//    typedef PVOID  (*AddHandler_t)(ULONG, PVECTORED_EXCEPTION_HANDLER);
//    typedef ULONG  (*RemoveHandler_t)(PVOID);
    typedef void *  (*AddHandler_t)(unsigned long, PVECTORED_EXCEPTION_HANDLER);
    typedef unsigned long  (*RemoveHandler_t)(void *);

    void * handler;
public:
    CrashHandler(bool bDisabled);
    ~CrashHandler();
};

#else

class CrashHandler
{
public:
    CrashHandler(bool /*bDisabled*/){};
    ~CrashHandler(){};
};

#endif

#endif
