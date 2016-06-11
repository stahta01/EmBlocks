/*1*/
//don't include this header, only configmanager-revision.cpp should do this.
#ifndef AUTOREVISION_H
#define AUTOREVISION_H


#include <wx/string.h>

namespace autorevision
{
	const unsigned int svn_revision = 2;
	const wxString svnRevision(_T("2"));
	const wxString svnDate(_T("2011-09-17 23:17:28"));
}



#endif
