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

/*
 * Objects of type InfoWindow are autonomous and must therefore always be instantiated using operator new.
 * In addition to this, InfoWindow does not have any non-static public class members.
 *
 * Since a (technically 100% correct) statement like
 *   new InfoWindow("foo", "bar");
 * is unintuitive, confusing or even objective to some people, this class uses a variation of the "Named Constructor Idiom".
 *
 * InfoWindow::Display("foo", "bar");
 * does the exact same thing as the above statement but looks a lot nicer.
 */

#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <wx/event.h>
#include <wx/timer.h>
#include <wx/string.h>

#if wxUSE_POPUPWIN
    #include <wx/popupwin.h>
    typedef wxPopupWindow wxInfoWindowBase;
#else
    #include "scrollingdialog.h"
    typedef wxScrollingDialog wxInfoWindowBase;
#endif

#undef new
#include <list>
#include <algorithm>

class Stacker
{
    std::list<int> widths;

    public:

    int StackMe(int mySize)
    {
        mySize += 3;
        int pos = 0;

        if(!widths.empty())
            pos = *(std::max_element(widths.begin(), widths.end()));

        widths.push_back(pos + mySize);
        return pos + mySize;
    };

    void ReleaseMe(int myPos)
    {
        std::list<int>::iterator it = std::find(widths.begin(), widths.end(), myPos);
        if(it != widths.end())
            widths.erase(it);
    };
};


class InfoWindow : public wxInfoWindowBase
{
    wxTimer *m_timer;
    int left;
    int top;
    int hMin;
    int pos;
    unsigned int status;
    unsigned int m_delay;
    unsigned int ks;
    std::list<wxString>::iterator my_message_iterator;
    static Stacker stacker;
    static int screenWidth;
    static int screenHeight;
    static std::list<wxString> active_messages; // if a new message is in this, don't display it (already is displayed). Check Display()

    InfoWindow(const wxString& title, const wxString& message, unsigned int delay, unsigned int hysteresis);
    virtual ~InfoWindow();
    void OnTimer(wxTimerEvent& e);
    void OnMove(wxMouseEvent& e);
    void OnClick(wxMouseEvent& e);
    DECLARE_EVENT_TABLE()

    public:

    static void Display(const wxString& title, const wxString& message, unsigned int delay = 5000, unsigned int hysteresis = 1);
};

#endif
