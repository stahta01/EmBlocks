/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU Lesser General Public License, version 3
 * http://www.gnu.org/licenses/lgpl-3.0.html
 *
 * $Revision: 4 $
 * $Id: sc_consts.cpp 4 2013-11-02 15:53:52Z gerard $
 * $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/sdk/scripting/bindings/sc_consts.cpp $
 */

#include <sdk_precomp.h>
#ifndef CB_PRECOMP
    #include <wx/string.h>
    #include <globals.h>
    #include <settings.h>
#endif

#include <filefilters.h>
#include "sc_base_types.h"

// helper macros to bind constants
#define BIND_INT_CONSTANT(a) SqPlus::BindConstant<SQInteger>(a, #a);
#define BIND_INT_CONSTANT_NAMED(a,n) SqPlus::BindConstant<SQInteger>(a, n);
#define BIND_WXSTR_CONSTANT_NAMED(a,n) BindVariable(const_cast<wxString*>(&a), n, SqPlus::VAR_ACCESS_CONSTANT);

namespace ScriptBindings
{
    wxString s_PathSep = wxFILE_SEP_PATH;

    void Register_Constants()
    {
        // platform constants
        BIND_INT_CONSTANT_NAMED(0, "PLATFORM_MSW");
        BIND_INT_CONSTANT_NAMED(1, "PLATFORM_GTK");
        BIND_INT_CONSTANT_NAMED(2, "PLATFORM_MAC");
        BIND_INT_CONSTANT_NAMED(3, "PLATFORM_OS2");
        BIND_INT_CONSTANT_NAMED(4, "PLATFORM_X11");
        BIND_INT_CONSTANT_NAMED(99, "PLATFORM_UNKNOWN");

        #ifdef __WXMSW__
            BIND_INT_CONSTANT_NAMED(0, "PLATFORM");
        #elif __WXGTK__
            BIND_INT_CONSTANT_NAMED(1, "PLATFORM");
        #elif __WXMAC__
            BIND_INT_CONSTANT_NAMED(2, "PLATFORM");
        #elif __WXOS2__
            BIND_INT_CONSTANT_NAMED(3, "PLATFORM");
        #elif __WXX11__
            BIND_INT_CONSTANT_NAMED(4, "PLATFORM");
        #else
            BIND_INT_CONSTANT_NAMED(99, "PLATFORM");
        #endif

        BIND_INT_CONSTANT_NAMED(PLUGIN_SDK_VERSION_MAJOR, "PLUGIN_SDK_VERSION_MAJOR");
        BIND_INT_CONSTANT_NAMED(PLUGIN_SDK_VERSION_MINOR, "PLUGIN_SDK_VERSION_MINOR");
        BIND_INT_CONSTANT_NAMED(PLUGIN_SDK_VERSION_RELEASE, "PLUGIN_SDK_VERSION_RELEASE");

        // path separator for filenames
        BIND_WXSTR_CONSTANT_NAMED(s_PathSep, "wxFILE_SEP_PATH");

        // dialog buttons
        BIND_INT_CONSTANT(wxOK);
        BIND_INT_CONSTANT(wxYES_NO);
        BIND_INT_CONSTANT(wxCANCEL);
        BIND_INT_CONSTANT(wxID_OK);
        BIND_INT_CONSTANT(wxID_YES);
        BIND_INT_CONSTANT(wxID_NO);
        BIND_INT_CONSTANT(wxID_CANCEL);
        BIND_INT_CONSTANT(wxICON_QUESTION);
        BIND_INT_CONSTANT(wxICON_INFORMATION);
        BIND_INT_CONSTANT(wxICON_WARNING);
        BIND_INT_CONSTANT(wxICON_ERROR);

        // wxPathFormat
        BIND_INT_CONSTANT(wxPATH_NATIVE);
        BIND_INT_CONSTANT(wxPATH_UNIX);
        BIND_INT_CONSTANT(wxPATH_BEOS);
        BIND_INT_CONSTANT(wxPATH_MAC);
        BIND_INT_CONSTANT(wxPATH_DOS);
        BIND_INT_CONSTANT(wxPATH_WIN);
        BIND_INT_CONSTANT(wxPATH_OS2);
        BIND_INT_CONSTANT(wxPATH_VMS);

        // for wxFileName::GetPath()
        BIND_INT_CONSTANT(wxPATH_GET_VOLUME);
        BIND_INT_CONSTANT(wxPATH_GET_SEPARATOR);

        // wxPathNormalize
        BIND_INT_CONSTANT(wxPATH_NORM_ENV_VARS);
        BIND_INT_CONSTANT(wxPATH_NORM_DOTS);
        BIND_INT_CONSTANT(wxPATH_NORM_TILDE);
        BIND_INT_CONSTANT(wxPATH_NORM_CASE);
        BIND_INT_CONSTANT(wxPATH_NORM_ABSOLUTE);
        BIND_INT_CONSTANT(wxPATH_NORM_LONG);
        BIND_INT_CONSTANT(wxPATH_NORM_SHORTCUT);
        BIND_INT_CONSTANT(wxPATH_NORM_ALL);

        // OptionsRelationType
        BIND_INT_CONSTANT(ortDeviceOptions);
        BIND_INT_CONSTANT(ortCompilerOptions);
        BIND_INT_CONSTANT(ortAssemblerOptions);
        BIND_INT_CONSTANT(ortLinkerOptions);
        BIND_INT_CONSTANT(ortIncludeDirs);
        BIND_INT_CONSTANT(ortLibDirs);

        // OptionsRelation
        BIND_INT_CONSTANT(orUseParentOptionsOnly);
        BIND_INT_CONSTANT(orUseTargetOptionsOnly);
        BIND_INT_CONSTANT(orPrependToParentOptions);
        BIND_INT_CONSTANT(orAppendToParentOptions);

        // TargetType
        BIND_INT_CONSTANT(ttExecutable);
        BIND_INT_CONSTANT(ttLibrary);

        // MakeCommand
        BIND_INT_CONSTANT(mcClean);
        BIND_INT_CONSTANT(mcDistClean);
        BIND_INT_CONSTANT(mcBuild);
        BIND_INT_CONSTANT(mcCompileFile);
        BIND_INT_CONSTANT(mcAskRebuildNeeded);
        BIND_INT_CONSTANT(mcSilentBuild);

        // PCHMode
        BIND_INT_CONSTANT(pchSourceDir);
        BIND_INT_CONSTANT(pchObjectDir);
        BIND_INT_CONSTANT(pchSourceFile);

        // printing scope for print dialog
        BIND_INT_CONSTANT(psSelection);
        BIND_INT_CONSTANT(psActiveEditor);
        BIND_INT_CONSTANT(psAllOpenEditors);

        // printing colour mode
        BIND_INT_CONSTANT(pcmBlackAndWhite);
        BIND_INT_CONSTANT(pcmColourOnWhite);
        BIND_INT_CONSTANT(pcmInvertColours);
        BIND_INT_CONSTANT(pcmAsIs);

        // TemplateOutputType
        BIND_INT_CONSTANT_NAMED(totProject, "wizProject");
        BIND_INT_CONSTANT_NAMED(totFiles, "wizFiles");
        BIND_INT_CONSTANT_NAMED(totCustom, "wizCustom");

        // SearchDirs
        BIND_INT_CONSTANT(sdHome);
        BIND_INT_CONSTANT(sdBase);
        BIND_INT_CONSTANT(sdTemp);
        BIND_INT_CONSTANT(sdPath);
        BIND_INT_CONSTANT(sdConfig);
        BIND_INT_CONSTANT(sdCurrent);
        BIND_INT_CONSTANT(sdPluginsUser);
        BIND_INT_CONSTANT(sdScriptsUser);
        BIND_INT_CONSTANT(sdDataUser);
        BIND_INT_CONSTANT(sdAllUser);
        BIND_INT_CONSTANT(sdPluginsGlobal);
        BIND_INT_CONSTANT(sdScriptsGlobal);
        BIND_INT_CONSTANT(sdDataGlobal);
        BIND_INT_CONSTANT(sdAllGlobal);
        BIND_INT_CONSTANT(sdAllKnown);

        // SupportedPlatforms
        BIND_INT_CONSTANT(spWindows);
        BIND_INT_CONSTANT(spUnix);
        BIND_INT_CONSTANT(spMac);
        BIND_INT_CONSTANT(spAll);

        // ModuleType
        BIND_INT_CONSTANT(mtProjectManager);
        BIND_INT_CONSTANT(mtEditorManager);
        BIND_INT_CONSTANT(mtLogManager);
        BIND_INT_CONSTANT(mtOpenFilesList);
        BIND_INT_CONSTANT(mtUnknown);

        // FileTreeDataKind
        BIND_INT_CONSTANT_NAMED(FileTreeData::ftdkUndefined, "ftdkUndefined");
        BIND_INT_CONSTANT_NAMED(FileTreeData::ftdkProject, "ftdkProject");
        BIND_INT_CONSTANT_NAMED(FileTreeData::ftdkFolder, "ftdkFolder");
        BIND_INT_CONSTANT_NAMED(FileTreeData::ftdkFile, "ftdkFile");
        BIND_INT_CONSTANT_NAMED(FileTreeData::ftdkVirtualGroup, "ftdkVirtualGroup");
        BIND_INT_CONSTANT_NAMED(FileTreeData::ftdkVirtualFolder, "ftdkVirtualFolder");

        // file extensions
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::WORKSPACE_EXT, "EXT_WORKSPACE");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::EMBLOCKS_EXT, "EXT_EMBLOCKS");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::MPLAB8PRJ_EXT, "EXT_MPLAB8PRJ");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::D_EXT, "EXT_D");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::H_EXT, "EXT_H");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::OBJECT_EXT, "EXT_OBJECT");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::LIBRARY_EXT, "EXT_LIBRARY");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::XML_EXT, "EXT_XML");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::SCRIPT_EXT, "EXT_SCRIPT");


        BIND_WXSTR_CONSTANT_NAMED(FileFilters::WORKSPACE_DOT_EXT, "DOT_EXT_WORKSPACE");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::EMBLOCKS_DOT_EXT, "DOT_EXT_EMBLOCKS");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::D_DOT_EXT, "DOT_EXT_D");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::H_DOT_EXT, "DOT_EXT_H");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::OBJECT_DOT_EXT, "DOT_EXT_OBJECT");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::LIBRARY_DOT_EXT, "DOT_EXT_LIBRARY");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::XML_DOT_EXT, "DOT_EXT_XML");
        BIND_WXSTR_CONSTANT_NAMED(FileFilters::SCRIPT_DOT_EXT, "DOT_EXT_SCRIPT");
         BIND_WXSTR_CONSTANT_NAMED(FileFilters::MPLAB8PRJ_DOT_EXT, "DOT_EXT_MPLAB8PRJ")
    }
};
