///////////////////////////////////////////////////////////////////////////////
// Name:        wxRSSReader.cpp
// Purpose:     RSS Reader for wxForum
// Author:      Ryan Norton
// Created:     06/27/2005
// RCS-ID:      $Id: rssreader.h 4 2013-11-02 15:53:52Z gerard $
// Copyright:   (c) Ryan Norton
// Licence:     Public Domain
///////////////////////////////////////////////////////////////////////////////
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

// ============================================================================
// Definitions
// ============================================================================

// ----------------------------------------------------------------------------
// Pre-compiled header stuff
// ----------------------------------------------------------------------------

#ifndef __RSS_READER__
#define __RSS_READER__

#include "wx/wxprec.h"
#include <sdk_events.h>
#include <toolsmanager.h>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------

// Version of wxRSSReader
#define WXRSSREADERVERSION 1.0

//Hack - if this is defined wxURL::GetInputStream is self-contained
//(it in not in wx 2.6.2)
#define GISC



// ----------------------------------------------------------------------------
// Headers
// ----------------------------------------------------------------------------

#include "wx/app.h"         //for wxApp
#include "wx/menu.h"        //for wxMenu
#include "wx/frame.h"       //for wxFrame
#include "wx/sizer.h"       //for positioning controls/wxBoxSizer
#include "wx/listctrl.h"    //for wxListCtrl
#include "wx/url.h"         //for wxURL/wxURI
#include "wx/gauge.h"       //for wxGauge
#include "wx/msgdlg.h"      //for wxMessageBox function
#include "wx/thread.h"      //for wxThread and wxCriticalSection
#include "wx/timer.h"       //for wxTimer
#include "wx/textdlg.h"     //for wxGetTextFromUser function
#include "wx/filename.h"    //for wxFileName
#include "wx/config.h"      //for wxConfigBase
#include "wx/dynarray.h"    //for wxArrayPtrVoid
#include "settings.h"


// ============================================================================
// Declarations
// ============================================================================


#define RSS_NEWS   0
#define RSS_UPDATE_BOX 1

// ----------------------------------------------------------------------------
//
// wxRSSFeedData - data stored for each feed
// wxRSSPostData - data stored for each post from a feed
//
// ----------------------------------------------------------------------------

struct wxRSSPostData
{
    wxString sTitle; //title of post
    wxString sLink;  //url to post
    wxString sData;  //content of post
    int iType;       // Normal news or notification boxes
};

WX_DECLARE_OBJARRAY(wxRSSPostData, wxRSSPostDataArray);

struct wxRSSFeedData
{
    wxString sLink;         //url of RSS feed
    wxRSSPostDataArray aPosts;  //posts (wxRSSPostData) in feed
    class wxRSSFeedThread*
           pLoadingThread;  //loading thread (NULL if not/done loading)
};

// ----------------------------------------------------------------------------
//
// wxRSSParser - simple XML parser
//
// ----------------------------------------------------------------------------

class wxRSSParser
{
public:
    bool Create(const wxString& loc);   //Load all data from a location
    void Create(wxInputStream* m_in);   //Load all data from stream
    wxString NextTag();                 //Scan to next tag in stream
    bool Ok();                          //Return true if still parsing
    wxString TagData();                 //Parse and return current tag data
    void HTMLUnencode(wxString& s);     //Unencode some complex XML/HTML
    void TagEnd(wxString tag);          //Scan past end of specified tag
    void Reset();                       //Scan to beginning of stream

    wxString m_data;    //raw RSS data from site
    wxString m_tag;     //content of currently parsed XML tag
    size_t m_nLastPos;  //current position in the RSS data
};

// ----------------------------------------------------------------------------
//
// wxRSSFrame - main frame
//
// ----------------------------------------------------------------------------

class wxRSSReader : public wxEvtHandler
{
public:
    //Ctor/Dtor
    wxRSSReader(wxFrame* parent = 0L);
    ~wxRSSReader();


    //Load a feed from a urlstring
    void LoadFeed(const wxString& s);

#ifdef __USE_RSS_INTERVAL_TIMER__
    void SetUpdateFeedInterval(int nInterval);
#endif

    //load one from the feed list/perform actual loading
    void UpdateFeed();

    wxRSSPostDataArray& GetFeedPosts() { return m_feedData.aPosts;}

    wxFrame* m_frame;

private:
    class wxRSSFeedThread*  m_thread;      //loading thread of current feed
    wxRSSFeedData m_feedData;         //current feed data

    //friends
#ifdef __USE_RSS_INTERVAL_TIMER__
    class wxRSSUpdateTimer* m_updatetimer; //updates all feeds periodically
    friend class wxRSSUpdateTimer;
#endif

};

// ----------------------------------------------------------------------------
//
// wxRSSUpdateTimer - constant timer to update all the feeds
//
// ----------------------------------------------------------------------------
#ifdef __USE_RSS_INTERVAL_TIMER__
class wxRSSUpdateTimer : public wxTimer
{
public:
    //Ctor/Dtor
    wxRSSUpdateTimer(wxRSSReader* reader);

    //Timer procedure
    virtual void Notify();
    wxRSSReader* m_pReader;
};
#endif
// ----------------------------------------------------------------------------
//
// wxRSSFeedThread - thread to retrieve data from site
//
// ----------------------------------------------------------------------------

class wxRSSFeedThread : public wxThread
{
public:
    wxRSSFeedThread(wxRSSFeedData* pFeedData, wxRSSReader* reader);
    virtual ExitCode Entry();

    wxRSSReader* m_pReader;
    wxRSSParser m_rssparser;      //Parser this thread is using
    wxRSSFeedData* m_pFeedData;   //Data relating to current feed
    wxInputStream* m_in;          //Stream to site
    wxCriticalSection m_cs;       //Locking critical section for access
};

extern const wxEventType ebEVT_RSS_FEED_READY;
#endif
