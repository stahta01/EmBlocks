///////////////////////////////////////////////////////////////////////////////
// Name:        wxRSSReader.cpp
// Purpose:     RSS Reader for wxForum
// Author:      Ryan Norton
// Created:     06/27/2005
// RCS-ID:      $Id: rssreader.cpp 4 2013-11-02 15:53:52Z gerard $
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

#include "rssreader.h"
#include <configmanager.h>
// ----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(wxRSSPostDataArray);

const wxEventType ebEVT_RSS_FEED_READY = wxNewEventType();

// ============================================================================
//
// Implementation
//
// ============================================================================

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSFrame
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSFrame Constructor
//
// 1) Create menus
// 2) Create controls
// 3) Load configuration   (wxConfigBase)
// 4) Connect events       (wxEvtHandler::Connect)
// 5) Create status bar
// 6) Start updating timer (wxRSSUpdateTimer)
// ----------------------------------------------------------------------------
wxRSSReader::wxRSSReader(wxFrame* parent)
    : wxEvtHandler(),
      m_frame(parent)
{
    wxSocketBase::Initialize();

    // Start the updating timer
#ifdef __USE_RSS_INTERVAL_TIMER__
    m_updatetimer = new wxRSSUpdateTimer(this);
    m_updatetimer->Start(RSS_UPDATE_TIME);
#endif

    m_feedData.sLink = _T("");
    m_feedData.pLoadingThread = NULL;
    m_feedData.aPosts.Empty();


    LoadFeed(_(RSS_URL "?version=" RELEASE "&rev=" REVERSION));
}


// ----------------------------------------------------------------------------
// wxRSSFrame::OnClose
//m_feedData
// 1) Stops and destroys the updating timer (wxRSSUpdateTimer)
// 2) Writes configuration so that the next time the app loads
//    it will start with the same list of feeds (wxConfigBase)
// ----------------------------------------------------------------------------
wxRSSReader::~wxRSSReader()
{
    //stop + destroy update timer and feed timer
#ifdef __USE_RSS_INTERVAL_TIMER__
    if(m_updatetimer)
    {
        m_updatetimer->Stop();
        delete m_updatetimer;
    }
#endif
    m_feedData.aPosts.Clear();
}



// ----------------------------------------------------------------------------
// wxRSSParser::OnSetUpdateFeedInterval
//
// Sets the interval our wxRSSUpdateTimer uses to update the feeds
// ----------------------------------------------------------------------------
#ifdef __USE_RSS_INTERVAL_TIMER__
void wxRSSReader::SetUpdateFeedInterval(int nInterval)
{
    m_updatetimer->Stop();
    m_updatetimer->Start(nInterval ? nInterval : RSS_UPDATE_TIME);
}
#endif



// ----------------------------------------------------------------------------
// wxRSSFrame::LoadFeed
//
// Loads a feed from a url
// ----------------------------------------------------------------------------
void wxRSSReader::LoadFeed(const wxString& url)
{
    if(m_feedData.pLoadingThread == NULL)
    {
        m_feedData.sLink = url;
        UpdateFeed();
    }
}

// ----------------------------------------------------------------------------
// wxRSSFrame::DoLoadFeed
//
// Performs actual loading of a feed from a feed number
// 1) Detaches frame from currently loading thread, if any
// 2) updates the list of posts in the main frame
// 3) loads the feed if it is not currently loading
// ----------------------------------------------------------------------------
void wxRSSReader::UpdateFeed()
{
    if(m_feedData.pLoadingThread == NULL)
    {
        m_feedData.aPosts.Clear();
        m_thread = new wxRSSFeedThread(&m_feedData, this);
        m_feedData.pLoadingThread = m_thread;

    }
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSUpdateTimer
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSUpdateTimer Constructor
//
// initializes members
// ----------------------------------------------------------------------------
#ifdef __USE_RSS_INTERVAL_TIMER__
wxRSSUpdateTimer::wxRSSUpdateTimer(wxRSSReader* reader)
    : m_pReader(reader)
{
}


// ----------------------------------------------------------------------------
// wxRSSUpdateTimer::Notify
//
// Timer procedure -
// Iterates through the feeds and updates them if they are not currently
//    updating (2nd param to DoLoadFeed signifies that we don't want to
//    update the GUI to reflect each feed switch)
// ----------------------------------------------------------------------------
void wxRSSUpdateTimer::Notify()
{
    m_pReader->UpdateFeed(); //load/update it!
}

#endif
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSFeedThread
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSFeedThread Constructor
//
// 1) Initializes members
// 2) Creates and Runs the actual thread
// ----------------------------------------------------------------------------
wxRSSFeedThread::wxRSSFeedThread(wxRSSFeedData* pFeedData, wxRSSReader* reader)
    : m_pReader(reader), m_pFeedData(pFeedData)
{
    Create();
    Run();
}

// ----------------------------------------------------------------------------
// wxRSSFeedThread::Entry
//
// Thread procedure -
// 1) Creates the RSS parser
// 2) Parses the XML RSS feed tag by tag until the "item" tag is reached,
//    at which point it puts parsed content from the tag into the post list
//    data
// 3) Signals the feed data that this thread and stopped loading
// ----------------------------------------------------------------------------
wxThread::ExitCode wxRSSFeedThread::Entry()
{
    // Load the data
    if( m_rssparser.Create(m_pFeedData->sLink) )
    {
        //Process the RSS content
        do
        {
            while(m_rssparser.Ok() &&
                    m_rssparser.NextTag() != wxT("item")) {}

            if(m_rssparser.Ok())
            {
                while(m_rssparser.Ok() &&
                        m_rssparser.NextTag() != wxT("title")) {}

                wxString szTitle = m_rssparser.TagData();
                while(m_rssparser.Ok() &&
                        m_rssparser.NextTag() != wxT("link")) {}
                wxString szLink = m_rssparser.TagData();
                while(m_rssparser.Ok() &&
                        m_rssparser.NextTag() != wxT("description")) {}
                wxString szPageHTML = m_rssparser.TagData();

                bool bFound = false;
                for(size_t i = 0; i < m_pFeedData->aPosts.GetCount(); ++i)
                {
                    wxRSSPostData* pPostData = &(m_pFeedData->aPosts[i]);

                    if(pPostData->sTitle == szTitle)
                    {
                        bFound  = true;
                        break;
                    }
                }

                if( bFound == false)
                {
                    wxRSSPostData PostData;
                    PostData.sTitle = szTitle;
                    PostData.sLink = szLink;
                    PostData.sData = szPageHTML;
                    PostData.iType = ( szPageHTML == _("UpdateMessageBox") ?  RSS_UPDATE_BOX : RSS_NEWS);
                    m_pFeedData->aPosts.Add(PostData);
                }
                m_rssparser.TagEnd(wxT("item"));
            }
        }
        while(m_rssparser.Ok() && !TestDestroy());
    }

    //Signal that we are done
    m_cs.Enter();
    m_pFeedData->pLoadingThread = NULL;
    m_cs.Leave();

    if(m_pReader->m_frame)
    {
        CodeBlocksEvent theEvent(ebEVT_RSS_FEED_READY);
        m_pReader->m_frame->AddPendingEvent(theEvent);
    }

    //done...
    return 0;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// wxRSSParser
//
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ----------------------------------------------------------------------------
// wxRSSParser::Create
//
// Connects and loads the data from the location
// ----------------------------------------------------------------------------
bool wxRSSParser::Create(const wxString& loc)
{
    wxURL url(loc);

    if (url.GetError() != wxURL_NOERR)
        return false;

    {
        // Set the proxy server if this is used
        wxString proxy = Manager::Get()->GetConfigManager( _T("app"))->GetProxy();
        if( proxy )
            url.SetProxy(proxy);
    }

    wxInputStream* m_in = url.GetInputStream();

    m_nLastPos = 0;
    size_t nRead;

    if(m_in)
    {
        //could use m_in->GetSize if not wxInvalidOffset or 0 also/instead
#define BUF_SIZE 100000
        char buf[BUF_SIZE];
        wxMBConv* conv = NULL;
        wxMBConv* realconv = NULL;
        bool bInit = true;
        do
        {
            nRead = m_in->Read(buf, BUF_SIZE).LastRead();

            //find encoding of stream - TEDIOUS
            if(bInit)
            {
                if(nRead < BUF_SIZE)
                    buf[nRead] = 0;
                if( memcmp(buf, "<?xml", 5) == 0 )
                {
                    char* b = buf;
                    while(nRead  - (buf-b) >= 8 &&
                            memcmp(b++, "encoding", 8) != 0) {}
                    if(*b)
                    {
                        b += 7;
                        while(*b++ != '=' && *b) {}
                        if(*b)
                        {
                            if(*b == '\"') ++b;
                            char* encb = b;
                            while(*b != ' ' && *b)
                            {
                                ++b;
                            }
                            if(*(--b - 1) == '\"') --b;
                            wxString sEnc = wxString(encb, wxConvLocal,
                                                     (size_t)(b - encb));
                            //wxMessageBox(sEnc);
                            conv = new wxCSConv(sEnc);
                        }
                    }
                }
                if(conv == NULL)
                    realconv = &wxConvUTF8;
                else
                    realconv = conv;
                bInit = false;
            }
            m_data += wxString(buf, *realconv, nRead);
        }
        while(nRead == BUF_SIZE);

        if(conv)
            delete conv;
        delete m_in;
        return true;
    }
    else
        return false;
}

// ----------------------------------------------------------------------------
// wxRSSParser::NextTag
//
// Scans to the next tag in the stream, returning the name of it
// ----------------------------------------------------------------------------
wxString wxRSSParser::NextTag()
{
    m_nLastPos = m_data.find(wxT('<'), m_nLastPos);

    if(m_nLastPos != wxString::npos)
    {
        m_nLastPos++;

        size_t nEndPos = m_data.find(wxT('>'), m_nLastPos) - m_nLastPos;
        size_t i = m_nLastPos;
        while(wxIsalpha(m_data[i]) && i != nEndPos)
        {
            ++i;
        }
        m_tag = m_data.substr(m_nLastPos, i - m_nLastPos);
        //wxMessageBox(m_tag);
        return m_tag;
    }
    else
        return wxT("");
}

// ----------------------------------------------------------------------------
// wxRSSParser::Ok
//
// Returns true if we are not at the end of the stream
// ----------------------------------------------------------------------------
bool wxRSSParser::Ok()
{
    return m_nLastPos != wxString::npos;
}

// ----------------------------------------------------------------------------
// wxRSSParser::TagData
//
// Parses the data of the current tag and unencodes it
// ----------------------------------------------------------------------------
wxString wxRSSParser::TagData()
{
    if(m_nLastPos == wxString::npos)
        return wxT("");

    m_nLastPos = m_data.find(wxT('>'), m_nLastPos) + 1;
    wxString sEndStr = wxString(wxT("</")+
                                m_tag + wxT(">"));
    size_t nTagEnd = m_data.find(sEndStr,
                                 m_nLastPos);

    wxString sRet;
    if(nTagEnd != wxString::npos)
    {
        sRet = m_data.substr(m_nLastPos, nTagEnd - m_nLastPos);
//            if(sRet.length() < 100)
//                    wxMessageBox(sRet);
        HTMLUnencode(sRet);

        m_nLastPos = nTagEnd + sEndStr.length();
    }
    else
        m_nLastPos = nTagEnd;
    return sRet;
}

// ----------------------------------------------------------------------------
// wxRSSParser::HTMLUnencode
//
// Converts HTML-encoded characters to "normal" ones
// FIXME
// FIXME: We still have problems with nested HTML tags in an &amp;
// FIXME
// ----------------------------------------------------------------------------
void wxRSSParser::HTMLUnencode(wxString& s)
{
    wxString sRet;

    for(size_t i = 0; i < s.length(); ++i)
    {
        if(s.c_str()[i] == wxT('&'))
        {
            if(s.length() - i >= 7)
            {
                if(s.c_str()[i] == wxT('&') &&
                        s.c_str()[i+1] == wxT('#') &&
                        wxIsdigit(s.c_str()[i+2]) &&
                        wxIsdigit(s.c_str()[i+3]) &&
                        wxIsdigit(s.c_str()[i+4]) &&
                        s.c_str()[i+5] == wxT(';'))
                {
                    sRet += ((wxChar)
                             (((s.c_str()[i+2] - wxT('0')) * 100) +
                              ((s.c_str()[i+3] - wxT('0')) * 10) +
                              ((s.c_str()[i+4] - wxT('0')) ) )
                            );
                    i += 6;
                    continue;
                }
            }
            if(s.length() - i >= 6)
            {
                if(wxTmemcmp(&s.c_str()[i], wxT("&apos;"), 6) == 0)
                {
                    sRet += wxT('\'');
                    i += 5;
                    continue;
                }
                if(wxTmemcmp(&s.c_str()[i], wxT("&nbsp;"), 6) == 0)
                {
                    sRet += wxT(' ');
                    i += 5;
                    continue;
                }
                if(wxTmemcmp(&s.c_str()[i], wxT("&quot;"), 6) == 0)
                {
                    sRet += wxT('\"');
                    i += 5;
                    continue;
                }
            }
            if(s.length() - i >= 5 &&
                    wxTmemcmp(&s.c_str()[i], wxT("&amp;"), 5) == 0)
            {
                sRet += wxT('&');
                i += 4; //FIXME:  We should re-scan like this:
//                    wxTmemmove((wxChar*)&s.c_str()[i+1],
//                                (wxChar*)&s.c_str()[i+5],
//                                s.length() - (i+5) );
//                    s.c_str()[i+5]
//                    i -= 1;
                continue;
            }
            if(s.length() - i >= 4)
            {
                if(wxTmemcmp(&s.c_str()[i], wxT("&lt;"), 4) == 0)
                {
                    sRet += wxT('<');
                    i += 3;
                    continue;
                }
                if(wxTmemcmp(&s.c_str()[i], wxT("&gt;"), 4) == 0)
                {
                    sRet += wxT('>');
                    i += 3;
                    continue;
                }
            }
        }

        sRet += s[i];
    }

    s = sRet;
}

// ----------------------------------------------------------------------------
// wxRSSParser::TagEnd
//
// Scans past the end of a specified tag
// ----------------------------------------------------------------------------
void wxRSSParser::TagEnd(wxString tag)
{
    if(m_nLastPos == wxString::npos)
        return;

    wxString sEndStr = wxString(wxT("</"))+
                       tag + wxT(">");

    size_t nTagEnd = m_data.find(sEndStr,
                                 m_nLastPos+1);
    if(nTagEnd != wxString::npos)
        m_nLastPos = nTagEnd + sEndStr.length();
    else
        m_nLastPos = nTagEnd;
}

// ----------------------------------------------------------------------------
// wxRSSParser::Reset
//
// Resets parsing to the beginning of the stream
// ----------------------------------------------------------------------------
void wxRSSParser::Reset()
{
    m_nLastPos = 0;
}

//
// End of wxRSSReader
//
