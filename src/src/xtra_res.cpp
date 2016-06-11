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

#ifndef CB_PRECOMP
    #include <wx/wx.h>
#endif

#include "xtra_res.h"

/////////////////////////////////////////////////////////////////////////////
// Name:        xh_toolb.cpp
// Purpose:     XRC resource for wxBoxSizer
// Author:      Vaclav Slavik
// Created:     2000/08/11
// RCS-ID:      $Id: xtra_res.cpp 4 2013-11-02 15:53:52Z gerard $
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
// Modified by Ricardo Garcia for Em::Blocks
// Comment: Things would've been much easier if field m_isInside had been
//          protected instead of private! >:(
/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//
//  wxToolBar
//
#if (0)
wxToolBarAddOnXmlHandler::wxToolBarAddOnXmlHandler()
: wxXmlResourceHandler(), m_isInside(FALSE), m_isAddon(false), m_toolbar(NULL)
{
    XRC_ADD_STYLE(wxTB_FLAT);
    XRC_ADD_STYLE(wxTB_DOCKABLE);
    XRC_ADD_STYLE(wxTB_VERTICAL);
    XRC_ADD_STYLE(wxTB_HORIZONTAL);
    XRC_ADD_STYLE(wxTB_3DBUTTONS);
    XRC_ADD_STYLE(wxTB_TEXT);
    XRC_ADD_STYLE(wxTB_NOICONS);
    XRC_ADD_STYLE(wxTB_NODIVIDER);
    XRC_ADD_STYLE(wxTB_NOALIGN);
}

wxBitmap wxToolBarAddOnXmlHandler::GetCenteredBitmap(const wxString& param,
    const wxArtClient& defaultArtClient, wxSize size)
{
    wxBitmap bitmap = GetBitmap(param, defaultArtClient, wxDefaultSize);
    if (!bitmap.Ok()) // == wxNullBitmap
        return bitmap;

    int bw = bitmap.GetWidth();
    int bh = bitmap.GetHeight();
    if (size == wxSize(bw, bh))
        return bitmap;

    wxImage image = bitmap.ConvertToImage();

    int w = size.GetWidth();
    int h = size.GetHeight();
    int x = (w - bw) / 2;
    int y = (h - bh) / 2;

    if (image.HasAlpha()) // Resize doesn't handle Alpha... :-(
    {
        const unsigned char *data = image.GetData();
        const unsigned char *alpha = image.GetAlpha();
        unsigned char *rgb = (unsigned char *) calloc(w * h, 3);
        unsigned char *a = (unsigned char *) calloc(w * h, 1);

        // copy Data/Alpha from smaller bitmap to larger bitmap
        for (int row = 0; row < bh; row++)
        {
            memcpy(rgb + ((row + y) * w + x) * 3, data + (row * bw) * 3, bw * 3);
            memcpy(a + ((row + y) * w + x), alpha + (row * bw), bw);
        }

        image = wxImage(w, h, rgb, a);
    }
    else
        image.Resize(size, wxPoint(x,y));

    return wxBitmap(image);
}

wxAuiToolBarItemArray prepend;
wxAuiToolBarItemArray append;

wxObject *wxToolBarAddOnXmlHandler::DoCreateResource()
{
    wxToolBar* toolbar=NULL;

    if (m_class == _T("tool"))
    {
        wxCHECK_MSG(m_toolbar, NULL, _("Incorrect syntax of XRC resource: tool not within a toolbar!"));

        wxSize bitmapSize = m_toolbar->GetToolBitmapSize();

        if (GetPosition() != wxDefaultPosition)
        {
            m_toolbar->AddTool(GetID(),
            #if wxCHECK_VERSION(2, 9, 0)
                               wxEmptyString,
            #endif
                               GetCenteredBitmap(_T("bitmap"), wxART_TOOLBAR, bitmapSize),
                               GetCenteredBitmap(_T("bitmap2"), wxART_TOOLBAR, bitmapSize),
            #if !wxCHECK_VERSION(2, 9, 0)
                               GetBool(_T("toggle")),
                               GetPosition().x,
                               GetPosition().y,
                               NULL,
            #else
                               wxITEM_NORMAL,
            #endif
                               GetText(_T("tooltip")),
                               GetText(_T("longhelp")));
           if (GetBool(_T("disabled")))
           {
               m_toolbar->Realize();
               m_toolbar->EnableTool(GetID(),false);
           }
        }
        else
        {
            wxItemKind kind = wxITEM_NORMAL;
            if (GetBool(_T("radio")))
                kind = wxITEM_RADIO;
            if (GetBool(_T("toggle")))
            {
                wxASSERT_MSG( kind == wxITEM_NORMAL,
                              _("can't have both toggleable and radion button at once") );
                kind = wxITEM_CHECK;
            }
            m_toolbar->AddTool(GetID(),
                               GetText(_T("label")),
                               GetCenteredBitmap(_T("bitmap"), wxART_TOOLBAR, bitmapSize),
                               GetCenteredBitmap(_T("bitmap2"), wxART_TOOLBAR, bitmapSize),
                               kind,
                               GetText(_T("tooltip")),
                               GetText(_T("longhelp")));

           if (GetBool(_T("disabled")))
           {
               m_toolbar->Realize();
               m_toolbar->EnableTool(GetID(),false);
           }
        }
        return m_toolbar; // must return non-NULL
    }

    else if (m_class == _T("separator"))
    {
        wxCHECK_MSG(m_toolbar, NULL, _("Incorrect syntax of XRC resource: separator not within a toolbar!"));
        m_toolbar->AddSeparator();
        return m_toolbar; // must return non-NULL
    }

    else /*<object class="wxToolBar">*/
    {
        m_isAddon=(m_class == _T("wxToolBarAddOn"));
        if(m_isAddon)
        { // special case: Only add items to toolbar
          toolbar=(wxToolBar*)m_instance;
          // XRC_MAKE_INSTANCE(toolbar, wxToolBar);
        }
        else
        {
            int style = GetStyle(_T("style"), wxNO_BORDER | wxTB_HORIZONTAL);
            #ifdef __WXMSW__
            if (!(style & wxNO_BORDER)) style |= wxNO_BORDER;
            #endif

            XRC_MAKE_INSTANCE(toolbar, wxToolBar)

            toolbar->Create(m_parentAsWindow,
                             GetID(),
                             GetPosition(),
                             GetSize(),
                             style,
                             GetName());
            wxSize bmpsize = GetSize(_T("bitmapsize"));
            if (!(bmpsize == wxDefaultSize))
                toolbar->SetToolBitmapSize(bmpsize);
            wxSize margins = GetSize(_T("margins"));
            if (!(margins == wxDefaultSize))
                toolbar->SetMargins(margins.x, margins.y);
            long packing = GetLong(_T("packing"), -1);
            if (packing != -1)
                toolbar->SetToolPacking(packing);
            long separation = GetLong(_T("separation"), -1);
            if (separation != -1)
                toolbar->SetToolSeparation(separation);

            prepend.Clear();
            append.Clear();
        }

        wxXmlNode *children_node = GetParamNode(_T("object"));
        if (!children_node)
           children_node = GetParamNode(_T("object_ref"));

        if (children_node == NULL) return toolbar;

        m_isInside = TRUE;
        m_toolbar = toolbar;

        wxXmlNode *n = children_node;

        while (n)
        {
            if ((n->GetType() == wxXML_ELEMENT_NODE) &&
                (n->GetName() == _T("object") || n->GetName() == _T("object_ref")))
            {
                wxObject *created = CreateResFromNode(n, toolbar, NULL);
                wxControl *control = wxDynamicCast(created, wxControl);
                if (!IsOfClass(n, _T("tool")) &&
                    !IsOfClass(n, _T("separator")) &&
                    control != NULL &&
                    control != toolbar)
                {
                    //Manager::Get()->GetLogManager()->DebugLog(F(_T("control=%p, parent=%p, toolbar=%p"), control, control->GetParent(), toolbar));
                    toolbar->AddControl(control);
                }
            }
            n = n->GetNext();
        }

        toolbar->Realize();

        toolbar->SetCustomOverflowItems( prepend, append);

        m_isInside = FALSE;
        m_toolbar = NULL;

        if(!m_isAddon)
        {
            if (m_parentAsWindow && !GetBool(_T("dontattachtoframe")))
            {
                wxFrame *parentFrame = wxDynamicCast(m_parent, wxFrame);
                if (parentFrame)
                    parentFrame->SetToolBar(toolbar);
            }
        }
        m_isAddon=false;
        return toolbar;
    }
}

bool wxToolBarAddOnXmlHandler::CanHandle(wxXmlNode *node)
{
// NOTE (mandrav#1#): wxXmlResourceHandler::IsOfClass() doesn't work in unicode (2.6.2)
// Don't ask why. It does this and doesn't work for our custom handler:
//    return node->GetPropVal(wxT("class"), wxEmptyString) == classname;
//
// This works though:
//    return node->GetPropVal(wxT("class"), wxEmptyString).Matches(classname);
//
// Don't ask me why... >:-|

    #if wxCHECK_VERSION(2, 9, 0)
    bool istbar = node->GetAttribute(wxT("class"), wxEmptyString).Matches(_T("wxToolBarAddOn"));
    bool istool = node->GetAttribute(wxT("class"), wxEmptyString).Matches(_T("tool"));
    bool issep = node->GetAttribute(wxT("class"), wxEmptyString).Matches(_T("separator"));
    #else
    bool istbar = node->GetPropVal(wxT("class"), wxEmptyString).Matches(_T("wxToolBarAddOn"));
    bool istool = node->GetPropVal(wxT("class"), wxEmptyString).Matches(_T("tool"));
    bool issep  = node->GetPropVal(wxT("class"), wxEmptyString).Matches(_T("separator"));
    #endif

    return ((!m_isInside && istbar) ||
            (m_isInside && istool) ||
            (m_isInside && issep));
}
#endif

////////////////////////////////////////////////////////////////////////////
//
//  wxAuiToolBar
//
wxAuiToolBarXmlHandler::wxAuiToolBarXmlHandler()
: wxXmlResourceHandler(), m_isInside(FALSE), m_isAddon(false), m_toolbar(NULL)
{

    XRC_ADD_STYLE(wxTB_FLAT);
    XRC_ADD_STYLE(wxTB_DOCKABLE);
    XRC_ADD_STYLE(wxTB_VERTICAL);
    XRC_ADD_STYLE(wxTB_HORIZONTAL);
    XRC_ADD_STYLE(wxTB_3DBUTTONS);
    XRC_ADD_STYLE(wxTB_TEXT);
    XRC_ADD_STYLE(wxTB_NOICONS);
    XRC_ADD_STYLE(wxTB_NODIVIDER);
    XRC_ADD_STYLE(wxTB_NOALIGN);

    XRC_ADD_STYLE(wxAUI_TB_TEXT);
    XRC_ADD_STYLE(wxAUI_TB_NO_TOOLTIPS);
    XRC_ADD_STYLE(wxAUI_TB_NO_AUTORESIZE);
    XRC_ADD_STYLE(wxAUI_TB_GRIPPER);
    XRC_ADD_STYLE(wxAUI_TB_OVERFLOW);
    XRC_ADD_STYLE(wxAUI_TB_VERTICAL);
    XRC_ADD_STYLE(wxAUI_TB_HORZ_LAYOUT);
    XRC_ADD_STYLE(wxAUI_TB_HORIZONTAL);
    XRC_ADD_STYLE(wxAUI_TB_HORZ_TEXT);
    XRC_ADD_STYLE(wxAUI_ORIENTATION_MASK);
    XRC_ADD_STYLE(wxAUI_TB_VERT_LAYOUT);
    XRC_ADD_STYLE(wxAUI_TB_ROTATION_90);
    XRC_ADD_STYLE(wxAUI_TB_ROTATION_270);
    XRC_ADD_STYLE(wxAUI_TB_ROTATION_AUTO);
    XRC_ADD_STYLE(wxAUI_TB_VERT_TEXT);
    XRC_ADD_STYLE(wxAUI_TB_SPIN_TEXT);
    XRC_ADD_STYLE(wxAUI_TB_DEFAULT_STYLE);
}

wxBitmap wxAuiToolBarXmlHandler::GetCenteredBitmap(const wxString& param,
    const wxArtClient& defaultArtClient, wxSize size)
{
    wxBitmap bitmap = GetBitmap(param, defaultArtClient, wxDefaultSize);
    if (!bitmap.Ok()) // == wxNullBitmap
        return bitmap;

    int bw = bitmap.GetWidth();
    int bh = bitmap.GetHeight();
    if (size == wxSize(bw, bh))
        return bitmap;

    wxImage image = bitmap.ConvertToImage();

    int w = size.GetWidth();
    int h = size.GetHeight();
    int x = (w - bw) / 2;
    int y = (h - bh) / 2;

    if (image.HasAlpha()) // Resize doesn't handle Alpha... :-(
    {
        const unsigned char *data = image.GetData();
        const unsigned char *alpha = image.GetAlpha();
        unsigned char *rgb = (unsigned char *) calloc(w * h, 3);
        unsigned char *a = (unsigned char *) calloc(w * h, 1);

        // copy Data/Alpha from smaller bitmap to larger bitmap
        for (int row = 0; row < bh; row++)
        {
            memcpy(rgb + ((row + y) * w + x) * 3, data + (row * bw) * 3, bw * 3);
            memcpy(a + ((row + y) * w + x), alpha + (row * bw), bw);
        }

        image = wxImage(w, h, rgb, a);
    }
    else
        image.Resize(size, wxPoint(x,y));

    return wxBitmap(image);
}

wxObject *wxAuiToolBarXmlHandler::DoCreateResource()
{
    wxAuiToolBar* toolbar=NULL;

    if (m_class == _T("tool"))
    {
        wxCHECK_MSG(m_toolbar, NULL, _("Incorrect syntax of XRC resource: tool not within a toolbar!"));

        wxSize bitmapSize = m_toolbar->GetToolBitmapSize();

        if (GetPosition() != wxDefaultPosition)
        {
            m_toolbar->AddTool(GetID(),
                               GetCenteredBitmap(_T("bitmap"), wxART_OTHER, wxSize(16,16)),
                               NULL, //GetCenteredBitmap(_T("bitmap2"), wxART_TOOLBAR, bitmapSize),
                               GetBool(_T("toggle")),
                               NULL,
                               GetText(_T("tooltip")),
                               GetText(_T("longhelp")));

           if (GetBool(_T("disabled")))
           {
               m_toolbar->Realize();
               m_toolbar->EnableTool(GetID(),false);
           }
        }
        else
        {
            wxItemKind kind = wxITEM_NORMAL;
            if (GetBool(_T("radio")))
                kind = wxITEM_RADIO;
            if (GetBool(_T("toggle")))
            {
                wxASSERT_MSG( kind == wxITEM_NORMAL,
                              _("can't have both toggleable and radion button at once") );
                kind = wxITEM_CHECK;
            }

            m_toolbar->AddTool(GetID(),
                               GetText(_T("label")),
                               GetCenteredBitmap(_T("bitmap"), wxART_OTHER, bitmapSize),
                               GetCenteredBitmap(_T("bitmap2"), wxART_OTHER, bitmapSize),
                               kind,
                               GetText(_T("tooltip")),
                               GetText(_T("longhelp")));

           if (GetBool(_T("disabled")))
           {
               m_toolbar->Realize();
               m_toolbar->EnableTool(GetID(),false);
           }
        }
        return m_toolbar; // must return non-NULL
    }

    else if (m_class == _T("overflow_tool"))
    {
        wxCHECK_MSG(m_toolbar, NULL, _("Incorrect syntax of XRC resource: tool not within a toolbar!"));

        wxSize bitmapSize = m_toolbar->GetToolBitmapSize();

        wxItemKind kind = wxITEM_NORMAL;
        if (GetBool(_T("radio")))
                kind = wxITEM_RADIO;
        if (GetBool(_T("toggle")))
        {
                wxASSERT_MSG( kind == wxITEM_NORMAL,
                              _("can't have both toggleable and radion button at once") );
                kind = wxITEM_CHECK;
        }
        if (GetBool(_T("separator")))
            kind = wxITEM_SEPARATOR;

        m_toolbar->AddOverflowTool(GetID(),
                               GetText(_T("label")),
                               GetCenteredBitmap(_T("bitmap"), wxART_OTHER, bitmapSize),
                               GetText(_T("longhelp")),
                               kind,
                               GetBool(_T("prepend"),0)   );

        if (GetBool(_T("disabled")))
        {
               m_toolbar->Realize();
               m_toolbar->EnableTool(GetID(),false);
        }

        return m_toolbar; // must return non-NULL
    }

    else if (m_class == _T("separator"))
    {
        wxCHECK_MSG(m_toolbar, NULL, _("Incorrect syntax of XRC resource: separator not within a toolbar!"));
        m_toolbar->AddSeparator();
        return m_toolbar; // must return non-NULL
    }

    else /*<object class="wxAuiToolBar">*/
    {
        toolbar=(wxAuiToolBar*)m_instance;
        if(!toolbar && (m_class == _T("wxToolBarAddOn")))
        {
           toolbar = new wxAuiToolBar(m_parentAsWindow, -1, wxDefaultPosition, wxDefaultSize);
           m_instance = (wxObject *)toolbar;
        }

            long style = GetStyle(_T("style"), -1);
            if (style!=-1)
                toolbar->SetWindowStyle(style);

            style = GetStyle(_T("add_style"), -1);
            if (style!=-1)
                toolbar->SetWindowStyle(toolbar->GetWindowStyle() | style);

            style = GetStyle(_T("remove_style"), -1);
            if (style!=-1)
                toolbar->SetWindowStyle(toolbar->GetWindowStyle() & ~style);

            wxSize bmpsize = GetSize(_T("bitmapsize"));
            // First find the generic bitmap size
            if (!(bmpsize == wxDefaultSize))
                toolbar->SetToolBitmapSize(bmpsize);
            else
            {
                // if no generic size then find the platform dependent size
                #ifdef __WXMSW__
                    bmpsize = GetSize(_T("bitmapsize_win"));
                #endif
                #ifdef __WXMAC__
                    bmpsize = GetSize(_T("bitmapsize_mac"));
                #endif
                #ifdef __WXGTK__
                    bmpsize = GetSize(_T("bitmapsize_unix"));
                #endif

                if (!(bmpsize == wxDefaultSize))
                    toolbar->SetToolBitmapSize(bmpsize);
            }

            wxSize margins = GetSize(_T("margins"));
            if (!(margins == wxDefaultSize))
                toolbar->SetMargins(margins.x, margins.y);
            long packing = GetLong(_T("packing"), -1);
            if (packing != -1)
                toolbar->SetToolPacking(packing);
            long separation = GetLong(_T("separation"), -1);
            if (separation != -1)
                toolbar->SetToolSeparation(separation);

        wxXmlNode *children_node = GetParamNode(_T("object"));
        if (!children_node)
           children_node = GetParamNode(_T("object_ref"));

        if (children_node == NULL) return toolbar;

        m_isInside = TRUE;
        m_toolbar = toolbar;

        wxXmlNode *n = children_node;

        while (n)
        {
            if ((n->GetType() == wxXML_ELEMENT_NODE) &&
                (n->GetName() == _T("object") || n->GetName() == _T("object_ref")))
            {
                wxObject *created = CreateResFromNode(n, toolbar, NULL);
                wxControl *control = wxDynamicCast(created, wxControl);
                if (!IsOfClass(n, _T("tool")) &&
                    !IsOfClass(n, _T("separator")) &&
                    control != NULL &&
                    control != toolbar)
                {
                    //Manager::Get()->GetLogManager()->DebugLog(F(_T("control=%p, parent=%p, toolbar=%p"), control, control->GetParent(), toolbar));
                    toolbar->AddControl(control);
                }
            }
            n = n->GetNext();
        }
        toolbar->Realize();
        m_isInside = FALSE;
        m_toolbar = NULL;
        m_isAddon=false;
        return toolbar;
    }
}

bool wxAuiToolBarXmlHandler::CanHandle(wxXmlNode *node)
{
// NOTE (mandrav#1#): wxXmlResourceHandler::IsOfClass() doesn't work in unicode (2.6.2)
// Don't ask why. It does this and doesn't work for our custom handler:
//    return node->GetPropVal(wxT("class"), wxEmptyString) == classname;
//
// This works though:
//    return node->GetPropVal(wxT("class"), wxEmptyString).Matches(classname);
//
// Don't ask me why... >:-|

    #if wxCHECK_VERSION(2, 9, 0)
    bool istbar = node->GetAttribute(wxT("class"), wxEmptyString).Matches(_T("wxToolBarAddOn"));
    bool istool = node->GetAttribute(wxT("class"), wxEmptyString).Matches(_T("tool"));
    bool issep = node->GetAttribute(wxT("class"), wxEmptyString).Matches(_T("separator"));
    #else
    wxString tmp = node->GetPropVal(wxT("class"), wxEmptyString);
    bool _istbar = node->GetPropVal(wxT("class"), wxEmptyString).Matches(_T("wxToolBarAddOn"));
    bool _istool = node->GetPropVal(wxT("class"), wxEmptyString).Matches(_T("tool"));
    bool _issep  = node->GetPropVal(wxT("class"), wxEmptyString).Matches(_T("separator"));
    bool _isoverflow  = node->GetPropVal(wxT("class"), wxEmptyString).Matches(_T("overflow_tool"));
    #endif

    return ((!m_isInside && _istbar) ||
            (m_isInside && _istool) ||
            (m_isInside && _issep) ||
            (m_isInside && _isoverflow)) ;
}

