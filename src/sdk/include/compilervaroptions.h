/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 */

#ifndef COMPILERVAROPTIONS_H
#define COMPILERVAROPTIONS_H

#include <wx/dynarray.h>
#include <wx/intl.h>
#include <wx/string.h>
#include "settings.h"


#define FLAG_EXCL_OPT          0x0100
#define FLAG_CMB_LEADER_OPT    0x0200
#define FLAG_CMB_MEMBER_OPT    0x0400

#define MASK_GROUP_OPT         0x00FF

struct CompVarOption
{
	// following comments are an example of an option
	wxString name;		//
	wxString macro;		//
	wxString format;	//
	wxString toolTip;	//
	wxString value;		//
};

WX_DEFINE_ARRAY(CompVarOption*, OptionsVarArray);

class DLLIMPORT CompilerVarOptions
{
	public:
		CompilerVarOptions();
		CompilerVarOptions(const CompilerVarOptions& other);
		CompilerVarOptions& operator=(const CompilerVarOptions& other);
		~CompilerVarOptions();
		void ClearOptions();
		void AddOption(CompVarOption* coption);
		void AddOption(const wxString& name,
                       const wxString& macro,
                       const wxString& format,
                       const wxString& toolTip = _(""),
                       const wxString& value = _(""));
		unsigned int GetCount() const { return m_Options.GetCount(); }
		CompVarOption* GetOption(int index){ return m_Options[index]; }
		CompVarOption* GetOptionByMacro(const wxString& macro);
		CompVarOption* GetOptionByName(const wxString& name);
		//CompVarOption* GetOptionByOption(const wxString& option);
	protected:
	private:
		OptionsVarArray m_Options;
};

#endif // COMPILEROPTIONS_H
