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
#include <wx/confbase.h>
#include <wx/fileconf.h>
#include <wx/intl.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/stopwatch.h>
#include "manager.h"
#include "configmanager.h"
#include "projectmanager.h"
#include "logmanager.h"
#include "macrosmanager.h"
#include "cbproject.h"
#include "compilerfactory.h"
#include "globals.h"
#endif

#include "settings.h"
#include "filefilters.h"
#include "projectloader.h"
#include "projectloader_hooks.h"
#include "annoyingdialog.h"
#include "configmanager.h"
#include "tinyxml/tinywxuni.h"

ProjectLoader::ProjectLoader(cbProject* project)
    : m_pProject(project),
      m_Upgraded(false),
      m_OpenDirty(false)
{
    //ctor
}

ProjectLoader::~ProjectLoader()
{
    //dtor
}

bool ProjectLoader::Open(const wxString& filename)
{
    return Open(filename, 0);
}

bool ProjectLoader::Open(const wxString& filename, TiXmlElement** ppExtensions)
{
    LogManager* pMsg = Manager::Get()->GetLogManager();
    if (!pMsg)
        return false;

    wxStopWatch sw;
    pMsg->DebugLog(_T("Loading project file..."));
    TiXmlDocument doc;
    if (!TinyXML::LoadDocument(filename, &doc))
        return false;

    pMsg->DebugLog(_T("Parsing project file..."));
    TiXmlElement* root;
    TiXmlElement* proj;

    root = doc.FirstChildElement("EmBlocks_project_file");
    if (!root)
    {
        pMsg->DebugLog(_T("Not a valid Em::Blocks project file..."));
        return false;
    }
    proj = root->FirstChildElement("Project");
    if (!proj)
    {
        pMsg->DebugLog(_T("No 'Project' element in file..."));
        return false;
    }

    TiXmlElement* version = root->FirstChildElement("FileVersion");

    // don't show messages if we 're running a batch build (i.e. no gui)
    if (!Manager::IsBatchBuild() && version)
    {
        int major = PROJECT_FILE_VERSION_MAJOR;
        int minor = PROJECT_FILE_VERSION_MINOR;
        version->QueryIntAttribute("major", &major);
        version->QueryIntAttribute("minor", &minor);

        unsigned int uiVersion = (major*10)+minor;

        if ( uiVersion > ((PROJECT_FILE_VERSION_MAJOR *10) +PROJECT_FILE_VERSION_MINOR) )
        {
            pMsg->DebugLog(F(_T("Project version is > %d.%d. Trying to load..."), PROJECT_FILE_VERSION_MAJOR, PROJECT_FILE_VERSION_MINOR));
            AnnoyingDialog dlg(_("Project file format is newer/unknown"),
                               _("This project file was saved with a newer version of Em::Blocks.\n"
                                 "Will try to load, but you should make sure all the settings were loaded correctly..."),
                               wxART_WARNING,
                               AnnoyingDialog::OK,
                               wxID_OK);
            dlg.ShowModal();
        }


        if ( uiVersion < ((PROJECT_FILE_VERSION_MAJOR *10) +PROJECT_FILE_VERSION_MINOR) )
        {
            pMsg->DebugLog(F(_T("Project version is < %d.%d. Trying to load..."), PROJECT_FILE_VERSION_MAJOR, PROJECT_FILE_VERSION_MINOR));
            AnnoyingDialog dlg(_("Project file format is older"),
                               _("This project file was saved with an older version of Em::Blocks.\n"
                                 "This version doesn't have the capabilities to convert, please update this version..."),
                               wxART_WARNING,
                               AnnoyingDialog::OK,
                               wxID_OK);
            dlg.ShowModal();
            return false;
        }
    }

    DoProjectOptions(proj);
    DoBuild(proj);
    DoDeviceOptions(proj);
    DoCompilerOptions(proj);
    DoAssemblerOptions(proj);
    DoLinkerOptions(proj);
    DoIncludesOptions(proj);
    DoLibsOptions(proj);
    DoExtraCommands(proj);
    DoUnits(proj);


    if (ppExtensions)
        *ppExtensions = 0;

    // as a last step, run all hooked callbacks
    TiXmlElement* node = proj->FirstChildElement("Extensions");
    if (node)
    {
        if (ppExtensions)
            *ppExtensions = new TiXmlElement(*node);
        ProjectLoaderHooks::CallHooks(m_pProject, node, true);
    }

    pMsg->DebugLog(wxString(_T("Done loading project in ")) << wxString::Format(_T("%d"), (int) sw.Time()) << _T("ms"));
    return true;
}


void ProjectLoader::DoMakeCommands(TiXmlElement* parentNode, CompileTargetBase* target)
{
    if (!parentNode)
        return; // no options

    TiXmlElement* node;

    node = parentNode->FirstChildElement("Build");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcBuild, cbC2U(node->Attribute("command")));

    node = parentNode->FirstChildElement("CompileFile");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcCompileFile, cbC2U(node->Attribute("command")));

    node = parentNode->FirstChildElement("Clean");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcClean, cbC2U(node->Attribute("command")));

    node = parentNode->FirstChildElement("DistClean");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcDistClean, cbC2U(node->Attribute("command")));

    node = parentNode->FirstChildElement("AskRebuildNeeded");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcAskRebuildNeeded, cbC2U(node->Attribute("command")));

    node = parentNode->FirstChildElement("SilentBuild");
    if (node && node->Attribute("command"))
        target->SetMakeCommandFor(mcSilentBuild, cbC2U(node->Attribute("command")));
}

void ProjectLoader::DoVirtualTargets(TiXmlElement* parentNode)
{
    if (!parentNode)
        return;

    TiXmlElement* node = parentNode->FirstChildElement("Add");
    if (!node)
        return; // no virtual targets

    while (node)
    {
        if (node->Attribute("alias") && node->Attribute("targets"))
        {
            wxString alias = cbC2U(node->Attribute("alias"));
            wxString targets = cbC2U(node->Attribute("targets"));
            wxArrayString arr = GetArrayFromString(targets, _T(";"), true);

            m_pProject->DefineVirtualBuildTarget(alias, arr);
        }

        node = node->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoProjectOptions(TiXmlElement* parentNode)
{
    TiXmlElement* node = parentNode->FirstChildElement("Option");
    if (!node)
        return; // no options

    wxString title;
    wxString makefile;
    bool makefile_custom = false;
    wxString make_tool;
    wxString execution_dir;
    wxString defaultTarget;
    wxString compilerId = _T("gcc");
    bool extendedObjectNames = false;
    bool forceLowerCaseObject = true;
    wxArrayString vfolders;
    PCHMode pch_mode = pchObjectDir;
    bool showNotes = false;
    wxString notes;

    // loop through all options
    while (node)
    {
        if (node->Attribute("title"))
        {
            title = cbC2U(node->Attribute("title"));
            if (title.Trim().IsEmpty())
                title = _T("untitled");
        }

        else if (node->Attribute("makefile")) // there is only one attribute per option, so "else" is a safe optimisation
            makefile = cbC2U(node->Attribute("makefile"));

        else if (node->Attribute("makefile_is_custom"))
            makefile_custom = strncmp(node->Attribute("makefile_is_custom"), "1", 1) == 0;

        else if (node->Attribute("make_tool"))
            make_tool = cbC2U(node->Attribute("make_tool"));

        else if (node->Attribute("execution_dir"))
            execution_dir = cbC2U(node->Attribute("execution_dir"));

        else if (node->Attribute("default_target"))
            defaultTarget = cbC2U(node->Attribute("default_target"));

        else if (node->Attribute("compiler"))
            compilerId = GetValidCompilerID(cbC2U(node->Attribute("compiler")), _T("the project"));

        else if (node->Attribute("extended_obj_names"))
            extendedObjectNames = strncmp(node->Attribute("extended_obj_names"), "1", 1) == 0;

        else if (node->Attribute("force_lowcase_obj_names"))
            forceLowerCaseObject = strncmp(node->Attribute("force_lowcase_obj_names"), "1", 1) == 0;

        else if (node->Attribute("pch_mode"))
            pch_mode = (PCHMode)atoi(node->Attribute("pch_mode"));

        else if (node->Attribute("virtualFolders"))
            vfolders = GetArrayFromString(cbC2U(node->Attribute("virtualFolders")), _T(";"));

        else if (node->Attribute("show_notes"))
        {
            TiXmlHandle parentHandle(node);
            TiXmlText* t = (TiXmlText *) parentHandle.FirstChild("notes").FirstChild().Node();
            if (t)
                notes = cbC2U(t->Value());
            showNotes = !notes.IsEmpty() && strncmp(node->Attribute("show_notes"), "1", 1) == 0;
        }

        node = node->NextSiblingElement("Option");
    }

    m_pProject->SetTitle(title);
    m_pProject->SetMakefile(makefile);
    m_pProject->SetMakefileCustom(makefile_custom);
    m_pProject->SetMakeTool(make_tool);
    m_pProject->SetMakefileExecutionDir(execution_dir);
    m_pProject->SetCompilerID(compilerId);
    m_pProject->SetExtendedObjectNamesGeneration(extendedObjectNames);
    m_pProject->SetForceLowerCaseObject(forceLowerCaseObject);
    m_pProject->SetModeForPCH(pch_mode);
    m_pProject->SetVirtualFolders(vfolders);
    m_pProject->SetNotes(notes);
    m_pProject->SetShowNotesOnLoad(showNotes);

    DoMakeCommands(parentNode->FirstChildElement("MakeCommands"), m_pProject);
    DoVirtualTargets(parentNode->FirstChildElement("VirtualTargets"));
}

void ProjectLoader::DoBuild(TiXmlElement* parentNode)
{
    TiXmlElement* node = parentNode->FirstChildElement("Build");
    while (node)
    {
        TiXmlElement* opt = node->FirstChildElement("Script");
        while (opt)
        {
            if (opt->Attribute("file"))
                m_pProject->AddBuildScript(cbC2U(opt->Attribute("file")));

            opt = opt->NextSiblingElement("Script");
        }

        DoBuildTarget(node);
        DoEnvironment(node, m_pProject);
        node = node->NextSiblingElement("Build");
    }
}

void ProjectLoader::DoBuildTarget(TiXmlElement* parentNode)
{
    TiXmlElement* node = parentNode->FirstChildElement("Target");
    if (!node)
        return; // no options

    while (node)
    {
        ProjectBuildTarget* target = 0L;
        wxString title = cbC2U(node->Attribute("title"));
        if (!title.IsEmpty())
            target = m_pProject->AddBuildTarget(title);

        if (target)
        {
            Manager::Get()->GetLogManager()->DebugLog(_T("Loading target ") + title);
            DoBuildTargetOptions(node, target);
            DoDeviceOptions(node, target);
            DoCompilerOptions(node, target);
            DoAssemblerOptions(node, target);
            DoLinkerOptions(node, target);
            DoIncludesOptions(node, target);
            DoLibsOptions(node, target);
            DoExtraCommands(node, target);
            DoEnvironment(node, target);
        }

        node = node->NextSiblingElement("Target");
    }
}

void ProjectLoader::DoBuildTargetOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("Option");
    if (!node)
        return; // no options

    bool create_hex = false;
    wxString output;
    wxString working_dir;
    wxString obj_output;
    wxString deps_output;
    wxString deps;
    wxString added;
    int type = -1;
    wxString compilerId = m_pProject->GetCompilerID();
    wxString parameters;
    wxString hostApplication;
    int projectDeviceOptionsRelation = 1;  // Only 0 or 1
    int projectCompilerOptionsRelation = 3;
    int projectAssemblerOptionsRelation = 3;
    int projectLinkerOptionsRelation = 3;
    int projectIncludeDirsRelation = 3;
    int projectLibDirsRelation = 3;

    while (node)
    {
        if (node->Attribute("create_hex"))
            create_hex = (strncmp(node->Attribute("create_hex"), "1", 1) == 0);

        if (node->Attribute("output"))
            output = UnixFilename(cbC2U(node->Attribute("output")));

        if (node->Attribute("working_dir"))
            working_dir = UnixFilename(cbC2U(node->Attribute("working_dir")));

        if (node->Attribute("object_output"))
            obj_output = UnixFilename(cbC2U(node->Attribute("object_output")));

        if (node->Attribute("deps_output"))
            deps_output = UnixFilename(cbC2U(node->Attribute("deps_output")));

        if (node->Attribute("external_deps"))
            deps = UnixFilename(cbC2U(node->Attribute("external_deps")));

        if (node->Attribute("additional_output"))
            added = UnixFilename(cbC2U(node->Attribute("additional_output")));

        if (node->Attribute("type"))
            type = atoi(node->Attribute("type"));

        if (node->Attribute("compiler"))
            compilerId = GetValidCompilerID(cbC2U(node->Attribute("compiler")), target->GetTitle());

        if (node->Attribute("parameters"))
            parameters = cbC2U(node->Attribute("parameters"));

        if (node->Attribute("host_application"))
            hostApplication = UnixFilename(cbC2U(node->Attribute("host_application")));

        if (node->Attribute("projectDeviceOptionsRelation"))
            projectDeviceOptionsRelation = atoi(node->Attribute("projectDeviceOptionsRelation"));

        if (node->Attribute("projectCompilerOptionsRelation"))
            projectCompilerOptionsRelation = atoi(node->Attribute("projectCompilerOptionsRelation"));

        if (node->Attribute("projectAssemblerOptionsRelation"))
            projectAssemblerOptionsRelation = atoi(node->Attribute("projectAssemblerOptionsRelation"));

        if (node->Attribute("projectLinkerOptionsRelation"))
            projectLinkerOptionsRelation = atoi(node->Attribute("projectLinkerOptionsRelation"));

        if (node->Attribute("projectIncludeDirsRelation"))
            projectIncludeDirsRelation = atoi(node->Attribute("projectIncludeDirsRelation"));

        if (node->Attribute("projectLibDirsRelation"))
            projectLibDirsRelation = atoi(node->Attribute("projectLibDirsRelation"));


        node = node->NextSiblingElement("Option");
    }

    node = parentNode->FirstChildElement("Script");
    while (node)
    {
        if (node->Attribute("file"))
            target->AddBuildScript(cbC2U(node->Attribute("file")));

        node = node->NextSiblingElement("Script");
    }

    if (type != -1)
    {
        target->SetCompilerID(compilerId);
        target->SetTargetType((TargetType)type); // type *must* come before output filename!
        target->SetOutputFilename(output); // because if no filename defined, one will be suggested based on target type...
        target->SetCreateHex(create_hex);
//       if (!working_dir.IsEmpty())
//           target->SetWorkingDir(working_dir);
        if (!obj_output.IsEmpty())
            target->SetObjectOutput(obj_output);
        if (!deps_output.IsEmpty())
            target->SetDepsOutput(deps_output);
        target->SetExternalDeps(deps);
        target->SetAdditionalOutputFiles(added);
        target->SetOptionRelation(ortDeviceOptions, (OptionsRelation)projectDeviceOptionsRelation);
        target->SetOptionRelation(ortCompilerOptions, (OptionsRelation)projectCompilerOptionsRelation);
        target->SetOptionRelation(ortAssemblerOptions, (OptionsRelation)projectAssemblerOptionsRelation);
        target->SetOptionRelation(ortLinkerOptions, (OptionsRelation)projectLinkerOptionsRelation);
        target->SetOptionRelation(ortIncludeDirs, (OptionsRelation)projectIncludeDirsRelation);
        target->SetOptionRelation(ortLibDirs, (OptionsRelation)projectLibDirsRelation);

        DoMakeCommands(parentNode->FirstChildElement("MakeCommands"), target);
    }
}


void ProjectLoader::DoDeviceOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("Device");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = cbC2U(child->Attribute("option"));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddDeviceStrOption(option);
            else
                m_pProject->AddDeviceStrOption(option);
        }
        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoCompilerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("Compiler");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = cbC2U(child->Attribute("option"));
        wxString dir = cbC2U(child->Attribute("directory"));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddCompilerStrOption(option);
            else
                m_pProject->AddCompilerStrOption(option);
        }
        if (!dir.IsEmpty())
        {
            if (target)
                target->AddIncludeDir(dir);
            else
                m_pProject->AddIncludeDir(dir);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoAssemblerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("Assembler");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = cbC2U(child->Attribute("option"));
        wxString dir = cbC2U(child->Attribute("directory"));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddAssemblerStrOption(option);
            else
                m_pProject->AddAssemblerStrOption(option);
        }

        child = child->NextSiblingElement("Add");
    }
}




void ProjectLoader::DoLinkerOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("Linker");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = cbC2U(child->Attribute("option"));
        wxString dir = UnixFilename(cbC2U(child->Attribute("directory")));
        wxString lib = UnixFilename(cbC2U(child->Attribute("library")));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddLinkerStrOption(option);
            else
                m_pProject->AddLinkerStrOption(option);
        }
        if (!lib.IsEmpty())
        {
            if (target)
                target->AddLinkLib(lib);
            else
                m_pProject->AddLinkLib(lib);
        }
        if (!dir.IsEmpty())
        {
            if (target)
                target->AddLibDir(dir);
            else
                m_pProject->AddLibDir(dir);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoIncludesOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("IncludeDirs");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = UnixFilename(cbC2U(child->Attribute("option")));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddIncludeDir(option);
            else
                m_pProject->AddIncludeDir(option);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoLibsOptions(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("LibDirs");
    if (!node)
        return; // no options

    TiXmlElement* child = node->FirstChildElement("Add");
    while (child)
    {
        wxString option = cbC2U(child->Attribute("option"));
        if (!option.IsEmpty())
        {
            if (target)
                target->AddLibDir(option);
            else
                m_pProject->AddLibDir(option);
        }

        child = child->NextSiblingElement("Add");
    }
}

void ProjectLoader::DoExtraCommands(TiXmlElement* parentNode, ProjectBuildTarget* target)
{
    TiXmlElement* node = parentNode->FirstChildElement("ExtraCommands");
    while (node)
    {
        CompileOptionsBase* base = target ? target : (CompileOptionsBase*)m_pProject;

        TiXmlElement* child = node->FirstChildElement("Add");
        while (child)
        {
            wxString before;
            wxString after;

            if (child->Attribute("before"))
                before = cbC2U(child->Attribute("before"));
            if (child->Attribute("after"))
                after = cbC2U(child->Attribute("after"));

            if (!before.IsEmpty())
                base->AddCommandsBeforeBuild(before);
            if (!after.IsEmpty())
                base->AddCommandsAfterBuild(after);

            child = child->NextSiblingElement("Add");
        }

        child = node->FirstChildElement("Mode");
        while (child)
        {
            if (child->Attribute("before"))
                base->SetPreBuildRunSettings(atoi(child->Attribute("before")));
            else if (child->Attribute("after"))
                base->SetPostBuildRunSettings(atoi(child->Attribute("after")));

            child = child->NextSiblingElement("Mode");
        }


        node = node->NextSiblingElement("ExtraCommands");
    }
}

void ProjectLoader::DoEnvironment(TiXmlElement* parentNode, CompileOptionsBase* base)
{
    if (!base)
        return;
    TiXmlElement* node = parentNode->FirstChildElement("Environment");
    while (node)
    {
        TiXmlElement* child = node->FirstChildElement("Variable");
        while (child)
        {
            wxString name = cbC2U(child->Attribute("name"));
            wxString value = cbC2U(child->Attribute("value"));
            if (!name.IsEmpty())
                base->SetVar(name, UnixFilename(value));

            child = child->NextSiblingElement("Variable");
        }
        node = node->NextSiblingElement("Environment");
    }
}

void ProjectLoader::DoUnits(TiXmlElement* parentNode)
{
    Manager::Get()->GetLogManager()->DebugLog(_T("Loading project files..."));
    m_pProject->BeginAddFiles();
    int count = 0;
    TiXmlElement* unit = parentNode->FirstChildElement("Unit");
    while (unit)
    {
        wxString filename = cbC2U(unit->Attribute("filename"));
        if (!filename.IsEmpty())
        {
            ProjectFile* file = m_pProject->AddFile(-1, UnixFilename(filename));
            if (!file)
                Manager::Get()->GetLogManager()->DebugLog(_T("Can't load file ") + filename);
            else
            {
                ++count;
                DoUnitOptions(unit, file);
            }
        }

        unit = unit->NextSiblingElement("Unit");
    }
    m_pProject->EndAddFiles();
    Manager::Get()->GetLogManager()->DebugLog(F(_T("%d files loaded"), count));
}

void ProjectLoader::DoUnitOptions(TiXmlElement* parentNode, ProjectFile* file)
{
    int tempval = 0;
    bool foundTarget = false;
    bool noTarget = false;

//    Compiler* compiler = CompilerFactory::GetCompiler(m_pProject->GetCompilerID());

    TiXmlElement* node = parentNode->FirstChildElement("Option");
    while (node)
    {
        if (node->Attribute("compilerVar"))
        {
            file->compilerVar = cbC2U(node->Attribute("compilerVar"));
        }
        //
        if (node->QueryIntAttribute("compile", &tempval) == TIXML_SUCCESS)
        {
            file->compile = tempval != 0;
        }
        //
        if (node->QueryIntAttribute("link", &tempval) == TIXML_SUCCESS)
        {
            file->link = tempval != 0;
        }
        //
        if (node->QueryIntAttribute("weight", &tempval) == TIXML_SUCCESS)
            file->weight = tempval;
        //
        if (node->Attribute("virtualFolder"))
        {
            file->virtual_path = UnixFilename(cbC2U(node->Attribute("virtualFolder")));
        }
        //
        if (node->Attribute("buildCommand") && node->Attribute("compiler"))
        {
            wxString cmp = cbC2U(node->Attribute("compiler"));
            wxString tmp = cbC2U(node->Attribute("buildCommand"));
            if (!cmp.IsEmpty() && !tmp.IsEmpty())
            {
                tmp.Replace(_T("\\n"), _T("\n"));
                file->SetCustomBuildCommand(cmp, tmp);
                if (node->QueryIntAttribute("use", &tempval) == TIXML_SUCCESS)
                    file->SetUseCustomBuildCommand(cmp, tempval != 0);
            }
        }
        //
        if (node->Attribute("target"))
        {
            wxString targetName = cbC2U(node->Attribute("target"));
            if (!targetName.IsSameAs(_T("<{~None~}>")))
            {
                file->AddBuildTarget(targetName);
                foundTarget = true;

                // All the per target file options
                {
                    TiXmlElement* tnode = node->FirstChildElement("Option");
                    if(tnode)
                    {
                        wxArrayString opts;
                        OptionsRelation optrel = (OptionsRelation)atoi(tnode->Attribute("CompilerOptionsRelation"));

                        file->SetOptionRelation(m_pProject->GetBuildTarget(targetName),optrel);

                        tnode = node->FirstChildElement("Add");
                        while(tnode)
                        {
                            opts.Add(cbC2U(tnode->Attribute("option")));
                            tnode = tnode->NextSiblingElement("Add");
                        }
                        file->SetCompilerStrOptions(m_pProject->GetBuildTarget(targetName) ,opts);
                    }
                }
            }
            else
                noTarget = true;
        }

        node = node->NextSiblingElement("Option");
    }

    if (!foundTarget && !noTarget)
    {
        // add to all targets
        for (int i = 0; i < m_pProject->GetBuildTargetsCount(); ++i)
        {
            file->AddBuildTarget(m_pProject->GetBuildTarget(i)->GetTitle());
        }

        // use same targets for generated files
        for (size_t n = 0; n < file->generatedFiles.size(); ++n)
        {
            for (int i = 0; i < m_pProject->GetBuildTargetsCount(); ++i)
            {
                file->generatedFiles[n]->AddBuildTarget(m_pProject->GetBuildTarget(i)->GetTitle());
            }
        }
    }
}

// convenience function, used in Save()
TiXmlElement* ProjectLoader::AddElement(TiXmlElement* parent, const char* name, const char* attr, const wxString& attribute)
{
    TiXmlElement elem(name);

    if (attr)
    {
        elem.SetAttribute(attr, cbU2C(attribute));
    }

    return parent->InsertEndChild(elem)->ToElement();
}

// convenience function, used in Save()
TiXmlElement* ProjectLoader::AddElement(TiXmlElement* parent, const char* name, const char* attr, int attribute)
{
    TiXmlElement elem(name);

    if (attr)
    {
        elem.SetAttribute(attr, attribute);
    }

    return parent->InsertEndChild(elem)->ToElement();
}

// convenience function, used in Save()
void ProjectLoader::AddArrayOfElements(TiXmlElement* parent, const char* name, const char* attr, const wxArrayString& array)
{
    if (!array.GetCount())
        return;
    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
        if (array[i].IsEmpty())
            continue;
        AddElement(parent, name, attr, array[i]);
    }
}

// convenience function, used in Save()
void ProjectLoader::SaveEnvironment(TiXmlElement* parent, CompileOptionsBase* base)
{
    if (!base)
        return;
    const StringHash& v = base->GetAllVars();
    if (v.empty())
        return;

    // explicitly sort the keys
    typedef std::map<wxString, wxString> SortedMap;
    SortedMap map;
    for (StringHash::const_iterator it = v.begin(); it != v.end(); ++it)
        map[it->first] = it->second;

    TiXmlElement* node = AddElement(parent, "Environment");
    for (SortedMap::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        TiXmlElement* elem = AddElement(node, "Variable", "name", it->first);
        elem->SetAttribute("value", cbU2C(it->second));
    }
}

bool ProjectLoader::Save(const wxString& filename)
{
    return Save(filename, 0);
}

bool ProjectLoader::Save(const wxString& filename, TiXmlElement* pExtensions)
{
    if (ExportTargetAsProject(filename, wxEmptyString, pExtensions))
    {
        m_pProject->SetModified(false);
        return true;
    }
    return false;
}

bool ProjectLoader::ExportTargetAsProject(const wxString& filename, const wxString& onlyTarget, TiXmlElement* pExtensions)
{
    const char* ROOT_TAG = "EmBlocks_project_file";

    TiXmlDocument doc;
    doc.SetCondenseWhiteSpace(false);
    doc.InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", "yes"));

    TiXmlElement* rootnode = static_cast<TiXmlElement*>(doc.InsertEndChild(TiXmlElement(ROOT_TAG)));
    if (!rootnode)
        return false;

//    Compiler* compiler = CompilerFactory::GetCompiler(m_pProject->GetCompilerID());

    //============================================================================
    //==============================  Header  ====================================
    //============================================================================

    rootnode->InsertEndChild(TiXmlElement("EmBlocksVersion"));
    rootnode->FirstChildElement("EmBlocksVersion")->SetAttribute("release", RELEASE);
    rootnode->FirstChildElement("EmBlocksVersion")->SetAttribute("revision", REVERSION);

    rootnode->InsertEndChild(TiXmlElement("FileVersion"));
    rootnode->FirstChildElement("FileVersion")->SetAttribute("major", PROJECT_FILE_VERSION_MAJOR);
    rootnode->FirstChildElement("FileVersion")->SetAttribute("minor", PROJECT_FILE_VERSION_MINOR);

    rootnode->InsertEndChild(TiXmlElement("Project"));
    TiXmlElement* prjnode = rootnode->FirstChildElement("Project");

    AddElement(prjnode, "Option", "title", m_pProject->GetTitle());
    if (m_pProject->GetMakefile() != _T("Makefile"))
        AddElement(prjnode, "Option", "makefile", m_pProject->GetMakefile());
    if (m_pProject->IsMakefileCustom())
        AddElement(prjnode, "Option", "makefile_is_custom", 1);
    if (m_pProject->GetMakeTool() != _T("make"))
        AddElement(prjnode, "Option", "make_tool", m_pProject->GetMakeTool());
    if (m_pProject->GetMakefileExecutionDir() != m_pProject->GetBasePath())
        AddElement(prjnode, "Option", "execution_dir", m_pProject->GetMakefileExecutionDir());
    if (m_pProject->GetModeForPCH() != pchObjectDir)
        AddElement(prjnode, "Option", "pch_mode", (int)m_pProject->GetModeForPCH());

    AddElement(prjnode, "Option", "compiler", m_pProject->GetCompilerID());
    if (m_pProject->GetVirtualFolders().GetCount() > 0)
        AddElement(prjnode, "Option", "virtualFolders", GetStringFromArray(m_pProject->GetVirtualFolders(), _T(";")));
    if (m_pProject->GetExtendedObjectNamesGeneration())
        AddElement(prjnode, "Option", "extended_obj_names", 1);
    if (m_pProject->GetForceLowerCaseObject() == 0)
        AddElement(prjnode, "Option", "force_lowcase_obj_names", 0);

    if (m_pProject->GetShowNotesOnLoad() || !m_pProject->GetNotes().IsEmpty())
    {
        TiXmlElement* notesBase = AddElement(prjnode, "Option", "show_notes", m_pProject->GetShowNotesOnLoad() ? 1 : 0);
        if (!m_pProject->GetNotes().IsEmpty())
        {
            TiXmlElement* notes = AddElement(notesBase, "notes");
            TiXmlText t(m_pProject->GetNotes().mb_str(wxConvUTF8));
            t.SetCDATA(true);
            notes->InsertEndChild(t);
        }
    }

    if (m_pProject->MakeCommandsModified())
    {
        TiXmlElement* makenode = AddElement(prjnode, "MakeCommands");
        AddElement(makenode, "Build", "command", m_pProject->GetMakeCommandFor(mcBuild));
        AddElement(makenode, "CompileFile", "command", m_pProject->GetMakeCommandFor(mcCompileFile));
        AddElement(makenode, "Clean", "command", m_pProject->GetMakeCommandFor(mcClean));
        AddElement(makenode, "DistClean", "command", m_pProject->GetMakeCommandFor(mcDistClean));
        AddElement(makenode, "AskRebuildNeeded", "command", m_pProject->GetMakeCommandFor(mcAskRebuildNeeded));
        AddElement(makenode, "SilentBuild", "command", m_pProject->GetMakeCommandFor(mcSilentBuild));
    }

    prjnode->InsertEndChild(TiXmlElement("Build"));
    TiXmlElement* buildnode = prjnode->FirstChildElement("Build");

    for (size_t x = 0; x < m_pProject->GetBuildScripts().GetCount(); ++x)
    {
        AddElement(buildnode, "Script", "file", m_pProject->GetBuildScripts().Item(x));
    }


    //============================================================================
    //============================== Targets  ====================================
    //============================================================================
    // now decide which target we 're exporting.
    // remember that if onlyTarget is empty, we export all targets (i.e. normal save).
    ProjectBuildTarget* onlytgt = m_pProject->GetBuildTarget(onlyTarget);

    for (int i = 0; i < m_pProject->GetBuildTargetsCount(); ++i)
    {
        ProjectBuildTarget* target = m_pProject->GetBuildTarget(i);
        if (!target)
            break;

        // skip every target except the desired one
        if (onlytgt && onlytgt != target)
            continue;

        TiXmlElement* tgtnode = AddElement(buildnode, "Target", "title", target->GetTitle());

        wxString outputFileName = target->GetOutputFilename();
        /*
                if (extensionPolicy == tgfpPlatformDefault)
                {
                        wxFileName fname(outputFileName);
                        fname.ClearExt();
                        outputFileName = fname.GetFullPath();
                }
        */
        //TiXmlElement* outnode =
        AddElement(tgtnode, "Option", "output", outputFileName);

//        if (target->GetWorkingDir() != _T("."))
//                AddElement(tgtnode, "Option", "working_dir", target->GetWorkingDir());
        if (target->GetObjectOutput() != _T(".objs"))
            AddElement(tgtnode, "Option", "object_output", target->GetObjectOutput());
        if (target->GetDepsOutput() != _T(".deps"))
            AddElement(tgtnode, "Option", "deps_output", target->GetDepsOutput());

        if (!target->GetExternalDeps().IsEmpty())
            AddElement(tgtnode, "Option", "external_deps", target->GetExternalDeps());
        if (!target->GetAdditionalOutputFiles().IsEmpty())
            AddElement(tgtnode, "Option", "additional_output", target->GetAdditionalOutputFiles());

        AddElement(tgtnode, "Option", "type", target->GetTargetType());

        if (target->GetCreateHex()  )
            AddElement(tgtnode, "Option", "create_hex", target->GetCreateHex() );

        AddElement(tgtnode, "Option", "compiler", target->GetCompilerID());
        if (target->GetOptionRelation(ortDeviceOptions) != 1) // 1 is the default  (0 and 1 is only possible)
            AddElement(tgtnode, "Option", "projectDeviceOptionsRelation", target->GetOptionRelation(ortDeviceOptions));
        if (target->GetOptionRelation(ortCompilerOptions) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectCompilerOptionsRelation", target->GetOptionRelation(ortCompilerOptions));
        if (target->GetOptionRelation(ortAssemblerOptions) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectAssemblerOptionsRelation", target->GetOptionRelation(ortAssemblerOptions));
        if (target->GetOptionRelation(ortLinkerOptions) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectLinkerOptionsRelation", target->GetOptionRelation(ortLinkerOptions));
        if (target->GetOptionRelation(ortIncludeDirs) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectIncludeDirsRelation", target->GetOptionRelation(ortIncludeDirs));
        if (target->GetOptionRelation(ortLibDirs) != 3) // 3 is the default
            AddElement(tgtnode, "Option", "projectLibDirsRelation", target->GetOptionRelation(ortLibDirs));

        for (size_t x = 0; x < target->GetBuildScripts().GetCount(); ++x)
        {
            AddElement(tgtnode, "Script", "file", target->GetBuildScripts().Item(x));
        }

        TiXmlElement* node = AddElement(tgtnode, "Device");
        AddArrayOfElements(node, "Add", "option", target->GetDeviceStrOptions());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);

        node = AddElement(tgtnode, "Compiler");
        AddArrayOfElements(node, "Add", "option", target->GetCompilerStrOptions());
        AddArrayOfElements(node, "Add", "directory", target->GetIncludeDirs());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);

        node = AddElement(tgtnode, "Assembler");
        AddArrayOfElements(node, "Add", "option", target->GetAssemblerStrOptions());
//GZa assembler include dirs
//        AddArrayOfElements(node, "Add", "directory", target->GetIncludeDirs());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);

        node = AddElement(tgtnode, "Linker");
        AddArrayOfElements(node, "Add", "option", target->GetLinkerStrOptions());
        AddArrayOfElements(node, "Add", "library", target->GetLinkLibs());
        AddArrayOfElements(node, "Add", "directory", target->GetLibDirs());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);

        node = AddElement(tgtnode, "ExtraCommands");
        AddArrayOfElements(node, "Add", "before", target->GetCommandsBeforeBuild());
        AddArrayOfElements(node, "Add", "after", target->GetCommandsAfterBuild());
        if (node->NoChildren())
            tgtnode->RemoveChild(node);
        else
        {
            AddElement(node, "Mode", "before", target->GetPreBuildRunSettings() );
            AddElement(node, "Mode", "after",  target->GetPostBuildRunSettings());
        }

        SaveEnvironment(tgtnode, target);

        if (target->MakeCommandsModified())
        {
            TiXmlElement* makenode = AddElement(tgtnode, "MakeCommands");
            AddElement(makenode, "Build", "command", target->GetMakeCommandFor(mcBuild));
            AddElement(makenode, "CompileFile", "command", target->GetMakeCommandFor(mcCompileFile));
            AddElement(makenode, "Clean", "command", target->GetMakeCommandFor(mcClean));
            AddElement(makenode, "DistClean", "command", target->GetMakeCommandFor(mcDistClean));
            AddElement(makenode, "AskRebuildNeeded", "command", target->GetMakeCommandFor(mcAskRebuildNeeded));
            AddElement(makenode, "SilentBuild", "command", target->GetMakeCommandFor(mcSilentBuild));
        }
    }

    //============================================================================
    //=================== virtuals only for whole projec =========================
    //============================================================================
    if (onlyTarget.IsEmpty())
    {
        TiXmlElement* virtnode = AddElement(prjnode, "VirtualTargets");
        wxArrayString virtuals = m_pProject->GetVirtualBuildTargets();
        for (size_t i = 0; i < virtuals.GetCount(); ++i)
        {
            const wxArrayString& group = m_pProject->GetVirtualBuildTargetGroup(virtuals[i]);
            wxString groupStr = GetStringFromArray(group, _T(";"));
            if (!groupStr.IsEmpty())
            {
                TiXmlElement* elem = AddElement(virtnode, "Add", "alias", virtuals[i]);
                elem->SetAttribute("targets", cbU2C(groupStr));
            }
        }
        if (virtnode->NoChildren())
            prjnode->RemoveChild(virtnode);
    }

    //============================================================================
    //===================== Project Settings =====================================
    //============================================================================
    SaveEnvironment(buildnode, m_pProject);

    TiXmlElement* node = AddElement(prjnode, "Device");
    AddArrayOfElements(node, "Add", "option", m_pProject->GetDeviceStrOptions());
    if (node->NoChildren())
        prjnode->RemoveChild(node);

    node = AddElement(prjnode, "Compiler");
    AddArrayOfElements(node, "Add", "option", m_pProject->GetCompilerStrOptions());
    AddArrayOfElements(node, "Add", "directory", m_pProject->GetIncludeDirs());
    if (node->NoChildren())
        prjnode->RemoveChild(node);

    node = AddElement(prjnode, "Assembler");
    AddArrayOfElements(node, "Add", "option", m_pProject->GetAssemblerStrOptions());
//GZa assembler search dirs
    //AddArrayOfElements(node, "Add", "directory", m_pProject->GetIncludeDirs());
    if (node->NoChildren())
        prjnode->RemoveChild(node);

    node = AddElement(prjnode, "Linker");
    AddArrayOfElements(node, "Add", "option", m_pProject->GetLinkerStrOptions());
    AddArrayOfElements(node, "Add", "library", m_pProject->GetLinkLibs());
    AddArrayOfElements(node, "Add", "directory", m_pProject->GetLibDirs());
    if (node->NoChildren())
        prjnode->RemoveChild(node);

    node = AddElement(prjnode, "ExtraCommands");
    AddArrayOfElements(node, "Add", "before", m_pProject->GetCommandsBeforeBuild());
    AddArrayOfElements(node, "Add", "after", m_pProject->GetCommandsAfterBuild());
    if (node->NoChildren())
        prjnode->RemoveChild(node);
    else
    {
        AddElement(node, "Mode", "before", m_pProject->GetPreBuildRunSettings() );
        AddElement(node, "Mode", "after",  m_pProject->GetPostBuildRunSettings());
    }

    //============================================================================
    //===================== File Settings =====================================
    //============================================================================
    int count = m_pProject->GetFilesCount();
    for (int i = 0; i < count; ++i)
    {
        ProjectFile* f = m_pProject->GetFile(i);

        // do not save auto-generated files
        if (f->AutoGeneratedBy())
        {
            continue;
        }
        // do not save project files that do not belong in the target we 're exporting
        if (onlytgt && !onlytgt->GetFilesList().Find(f))
        {
            continue;
        }
        FileType ft = FileTypeOf(f->relativeFilename);

        TiXmlElement* unitnode = AddElement(prjnode, "Unit", "filename", f->relativeFilename);
        if (!f->compilerVar.IsEmpty())
        {
            //  wxString ext = f->relativeFilename.AfterLast(_T('.')).Lower();
            //  if (f->compilerVar != _T("CC") && (ext.IsSameAs(FileFilters::C_EXT)))
            AddElement(unitnode, "Option", "compilerVar", f->compilerVar);
            //   else if (f->compilerVar != _T("CPP")) // default
            //      AddElement(unitnode, "Option", "compilerVar", f->compilerVar);
        }

        if (f->compile != (ft == ftSource || ft == ftAssembler))
        {
            AddElement(unitnode, "Option", "compile", f->compile ? 1 : 0);
        }
        if (f->link !=
                (ft == ftSource ||
                 ft == ftAssembler ||
                 ft == ftObject ||
                 ft == ftLibrary))
        {
            AddElement(unitnode, "Option", "link", f->link ? 1 : 0);
        }
        if (f->weight != 50)
            AddElement(unitnode, "Option", "weight", f->weight);
        if (!f->virtual_path.IsEmpty())
            AddElement(unitnode, "Option", "virtualFolder", f->virtual_path);

        // loop and save custom build commands
        for (pfCustomBuildMap::iterator it = f->customBuild.begin(); it != f->customBuild.end(); ++it)
        {
            pfCustomBuild& pfcb = it->second;
            if (!pfcb.buildCommand.IsEmpty())
            {
                wxString tmp = pfcb.buildCommand;
                tmp.Replace(_T("\n"), _T("\\n"));
                TiXmlElement* elem = AddElement(unitnode, "Option", "compiler", it->first);
                elem->SetAttribute("use", pfcb.useCustomBuildCommand ? "1" : "0");
                elem->SetAttribute("buildCommand", cbU2C(tmp));
            }
        }

        {
            bool fInAllTargets = ((int)f->buildTargets.GetCount() == m_pProject->GetBuildTargetsCount());

            for (unsigned int x = 0; x < f->buildTargets.GetCount(); ++x)
            {
                wxString targetName = f->buildTargets[x];
                ProjectBuildTarget* pTarget = m_pProject->GetBuildTarget(targetName);
                wxArrayString opts = f->GetCompilerStrOptions(pTarget);
                OptionsRelation optrel = f->GetOptionRelation(pTarget);

                bool fSaveFileOptions = ( (optrel != orUseParentOptionsOnly) || (opts.GetCount() > 0) );

                if(!fInAllTargets || fSaveFileOptions )
                {
                    TiXmlElement* targetnode = AddElement(unitnode, "Option", "target", targetName);
                    if(fSaveFileOptions)
                    {
                        AddElement(targetnode, "Option", "CompilerOptionsRelation", optrel);
                        // All the target compiler options
                        for(int i=0; i< opts.GetCount(); i++)
                            AddElement(targetnode, "Add", "option", opts[i]);
                    }
                }
            }
        }

        /* Add a target with a weird name if no targets are present. *
         * This will help us detecting a file with no targets.       */
        if ((int)f->buildTargets.GetCount() == 0)
        {
            AddElement(unitnode, "Option", "target", _T("<{~None~}>"));
        }
    }

    // as a last step, run all hooked callbacks
    TiXmlElement* extnode = pExtensions
                            ? prjnode->InsertEndChild(*pExtensions)->ToElement()
                            : AddElement(prjnode, "Extensions");
    if (ProjectLoaderHooks::HasRegisteredHooks() && extnode)
    {
        ProjectLoaderHooks::CallHooks(m_pProject, extnode, false);
    }

    return cbSaveTinyXMLDocument(&doc, filename);
}

wxString ProjectLoader::GetValidCompilerID(const wxString& proposal, const wxString& scope)
{
    if (CompilerFactory::GetCompiler(proposal))
        return proposal;

    // check the map; maybe we asked the user before
    CompilerSubstitutes::iterator it = m_CompilerSubstitutes.find(proposal);
    if (it != m_CompilerSubstitutes.end())
        return it->second;

    Compiler* compiler = 0;

    // if compiler is a number, then this is an older version of the project file
    // propose the same compiler by index
    if (!proposal.IsEmpty())
    {
        long int idx = -1;
        if (proposal.ToLong(&idx))
            compiler = CompilerFactory::GetCompiler(idx);
    }

    if (!compiler)
    {
        if(!(Manager::Get()->GetConfigManager(_T("app"))->ReadBool(_T("/environment/ignore_invalid_targets"), true)))
        {
            wxString msg;
            msg.Printf(_("The defined compiler for %s cannot be located (ID: %s).\n"
                         "Please choose the compiler you want to use instead and click \"OK\".\n"
                         "If you click \"Cancel\", the project/target will be excluded from the build."), scope.c_str(),
                       proposal.c_str());
            compiler = CompilerFactory::SelectCompilerUI(msg);
        }
    }

    if (!compiler)
    {
        // allow for invalid compiler IDs to be preserved...
        m_CompilerSubstitutes[proposal] = proposal;
        return proposal;
    }

    m_OpenDirty = true;

    // finally, keep the user selection in the map so we don't ask him again
    m_CompilerSubstitutes[proposal] = compiler->GetID();
    return compiler->GetID();
}
