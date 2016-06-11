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

#ifndef CB_PRECOMP
    #include <wx/choicdlg.h> // wxSingleChoiceDialog
    #include "compilerfactory.h"
    #include "manager.h"
    #include "logmanager.h"
    #include "configmanager.h"
    #include "compiler.h"
#endif

#include "autodetectcompilers.h"

// statics
CompilersArray CompilerFactory::Compilers;

size_t CompilerFactory::GetCompilersCount()
{
    return Compilers.GetCount();
}

Compiler* CompilerFactory::GetCompiler(size_t index)
{
    // NOTE: The index can be -1 , if there is no compiler at all or less than number of compilers.
    /* NOTE: Any negative value of index will be converted to a large unsigned integer.
    Therefore it's safe to check if the index equals or exceeds the compiler count. */
    if ((Compilers.GetCount() < 1) || (index >= Compilers.GetCount()))
        return 0;
    return Compilers[index];
}

Compiler* CompilerFactory::GetCompiler(const wxString& id)
{
    const wxString lid = id.Lower();
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        if (Compilers[i]->GetID().IsSameAs(lid))
        {
            return Compilers[i];
        }
    }
    return 0;
}

Compiler* CompilerFactory::GetCompilerByName(const wxString& title)
{
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        if (Compilers[i]->GetName().IsSameAs(title))
        {
            return Compilers[i];
        }
    }
    return 0;
}

int CompilerFactory::GetCompilerIndex(const wxString& id)
{
    const wxString lid = id.Lower();
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        if (Compilers[i]->GetID().IsSameAs(lid))
        {
            return i;
        }
    }
    return -1;
}

int CompilerFactory::GetCompilerIndex(Compiler* compiler)
{
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        if (Compilers[i] == compiler)
        {
            return i;
        }
    }
    return -1;
}

bool CompilerFactory::CompilerInheritsFrom(const wxString& id, const wxString& from_id)
{
    return CompilerInheritsFrom(GetCompiler(id), from_id);
}

bool CompilerFactory::CompilerInheritsFrom(Compiler* compiler, const wxString& from_id)
{
    if (!compiler)
        return false;

    wxString id = compiler->GetID();
    if (id.Matches(from_id))
        return true;

    while (compiler)
    {
        wxString id = compiler->GetParentID();
        if (id.Matches(from_id))
            return true;

        // traverse up the chain
        Compiler* newcompiler = GetCompiler(id);
        if (compiler == newcompiler)
        {
            Manager::Get()->GetLogManager()->DebugLog(_T("Compiler circular dependency detected?!?!?"));
            break;
        }
        compiler = newcompiler;
    }
    return false;
}

void CompilerFactory::RemoveCompiler(Compiler* compiler)
{
    if (!compiler || compiler->m_ParentID.IsEmpty())
        return;
    Manager::Get()->GetConfigManager(_T("compiler"))->DeleteSubPath(_T("/user_sets/") + compiler->GetID());

    Compilers.Remove(compiler);
    #if wxCHECK_VERSION(2, 9, 0)
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Compiler \"%s\" removed"), compiler->GetName().wx_str()));
    #else
    Manager::Get()->GetLogManager()->DebugLog(F(_T("Compiler \"%s\" removed"), compiler->GetName().c_str()));
    #endif

    Compiler::m_CompilerIDs.Remove(compiler->GetID());
    delete compiler;

    SaveSettings();
}

void CompilerFactory::UnregisterCompilers()
{
    WX_CLEAR_ARRAY(CompilerFactory::Compilers);
    CompilerFactory::Compilers.Empty();
    Compiler::m_CompilerIDs.Empty();
}


void CompilerFactory::SaveSettings()
{
	// clear old keys before saving
	Manager::Get()->GetConfigManager(_T("compiler"))->DeleteSubPath(_T("/sets"));
	Manager::Get()->GetConfigManager(_T("compiler"))->DeleteSubPath(_T("/user_sets"));

    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        wxString baseKey = Compilers[i]->GetParentID().IsEmpty() ? _T("/sets") : _T("/user_sets");
        Compilers[i]->SaveSettings(baseKey);
    }
}

void CompilerFactory::LoadSettings()
{
    bool needAutoDetection = false;
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        wxString baseKey = Compilers[i]->GetParentID().IsEmpty() ? _T("/sets") : _T("/user_sets");
        Compilers[i]->LoadSettings(baseKey);
        if (Compilers[i]->GetMasterPath(false).IsEmpty())
            needAutoDetection = true;
    }

    // auto-detect missing compilers
    if (needAutoDetection)
    {
        AutoDetectCompilers adc(Manager::Get()->GetAppWindow());
        PlaceWindow(&adc);
        adc.ShowModal();
    }
}

Compiler* CompilerFactory::SelectCompilerUI(const wxString& message, const wxString& preselectedID)
{
    int selected = -1;
    const wxString lid = preselectedID.Lower();

    // first build a list of available compilers
    wxString* comps = new wxString[Compilers.GetCount()];
    for (size_t i = 0; i < Compilers.GetCount(); ++i)
    {
        comps[i] = Compilers[i]->GetName();
    }
    // now display a choice dialog
    wxSingleChoiceDialog dlg(0,
                        message,
                        _("Compiler selection"),
                        CompilerFactory::Compilers.GetCount(),
                        comps);
    dlg.SetSelection(selected);
    PlaceWindow(&dlg);
    if (dlg.ShowModal() == wxID_OK)
        return Compilers[dlg.GetSelection()];
    return 0;
}

FileType CompilerFactory::GetFileTypeByCompilerTool(wxString& fileExt, const wxString& cmpId )
{
    if(const Compiler* pCompiler = GetCompiler(cmpId))
    {
       int result;
       CompilerTool ct = pCompiler->GetCompilerTool(fileExt, &result);
       if(result != -1)
       {
            if(  ct == pCompiler->GetCompilerTool(ctCompileObjectCmd))
                return ftSource;

            if(  ct == pCompiler->GetCompilerTool(ctCompileCppObjectCmd))
                return ftSource;

            if(  ct == pCompiler->GetCompilerTool(ctAssembleObjectCmd))
                return ftAssembler;
       }
    }
    return ftOther;
}

wxString CompilerFactory::GetCompilerVersionString(const wxString& Id)
{
	wxString Version;
	if(const Compiler* pCompiler = GetCompiler(Id))
	{
		Version = pCompiler->GetVersionString();
	}
	return Version;
} // end of GetCompilerVersionString
