#ifndef REOPENEDITORCONFDLG_H
#define REOPENEDITORCONFDLG_H

#include <configurationpanel.h>
//(*Headers(ReopenEditorConfDLg)
#include <wx/panel.h>
class wxRadioBox;
//*)


class ReopenEditorConfDLg: public cbConfigurationPanel
{
	public:

		ReopenEditorConfDLg(wxWindow* parent);
		virtual ~ReopenEditorConfDLg();

        virtual wxBitmap GetBitmap() { return wxXmlResource::Get()->LoadBitmap(_T("bmp_reopen"));}

		//(*Declarations(ReopenEditorConfDLg)
		wxRadioBox* RadioBox1;
		//*)

	protected:

		//(*Identifiers(ReopenEditorConfDLg)
		//*)

	private:

		//(*Handlers(ReopenEditorConfDLg)
		//*)
        wxString GetTitle() const { return _("Reopen editor"); }

        void OnApply() { SaveSettings(); }
        void OnCancel() {}
        void SaveSettings();

	protected:

		void BuildContent(wxWindow* parent);

		DECLARE_EVENT_TABLE()
};

#endif
