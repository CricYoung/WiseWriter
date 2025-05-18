// WiseWriter About 視窗（標準乾淨版）
#include <wx/wx.h>
#include <wx/hyperlink.h>

class DlgAbout : public wxDialog
{
public:
    DlgAbout(wxWindow* parent,const wxString& title=_("About"))
        : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300))
    {
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        // App Icon (Optional)
        // wxBitmap appIcon("icon.png", wxBITMAP_TYPE_PNG);
        // wxStaticBitmap* icon = new wxStaticBitmap(this, wxID_ANY, appIcon);
        // mainSizer->Add(icon, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

        // App Name
        wxStaticText* appName = new wxStaticText(this, wxID_ANY, "WiseWriter");
        wxFont font = appName->GetFont();
        font.SetPointSize(14);
        font.SetWeight(wxFONTWEIGHT_BOLD);
        appName->SetFont(font);
        mainSizer->Add(appName, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

        // Version
        wxStaticText* version = new wxStaticText(this, wxID_ANY, "Version 1.0.0");
        mainSizer->Add(version, 0, wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 10);

        // Copyright
        wxStaticText* copyright = new wxStaticText(this, wxID_ANY, 
            "© 2025 Cric. All rights reserved.");
        mainSizer->Add(copyright, 0, wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 10);

        // Website Link (Optional)
        wxHyperlinkCtrl* website = new wxHyperlinkCtrl(this, wxID_ANY, 
            "www.wisewriter.app", "https://www.wisewriter.app");
        mainSizer->Add(website, 0, wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 10);

        // Open Source Notice (Optional)
        wxStaticText* openSource = new wxStaticText(this, wxID_ANY, 
            "Built with wxWidgets (LGPL). Thank you open source community.");
        openSource->Wrap(350);
        mainSizer->Add(openSource, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

        // OK button
        mainSizer->Add(CreateButtonSizer(wxOK), 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

        SetSizerAndFit(mainSizer);
    }
};

// 使用範例
// DlgAbout* dlg = new DlgAbout(this);
// dlg->ShowModal();
// dlg->Destroy();
