#include <wx/wx.h>
#include <wx/cmdline.h>

class AnsiConvApp: public wxApp
{
public:
  virtual bool OnInit();
  virtual void OnInitCmdLine(wxCmdLineParser &);
  virtual bool OnCmdLineParsed(wxCmdLineParser &);

private:
  wxString input_name;
  wxString output_name;
};

bool
AnsiConvApp::OnInit()
{
  wxLog::SetActiveTarget(new wxLogStderr);
  return wxApp::OnInit();
}

void
AnsiConvApp::OnInitCmdLine(wxCmdLineParser &parser)
{
  parser.AddOption(wxT_2("i"),
                   wxT_2("input"),
                   wxT_2("The input ansi art file."));
  parser.AddOption(wxT_2("o"),
                   wxT_2("output"),
                   wxT_2("The output picture."));
}

bool
AnsiConvApp::OnCmdLineParsed(wxCmdLineParser &parser)
{
  if (!parser.Found(wxT_2("i"), &input_name)) {
    wxLogError(wxT_2("Input not specified."));
    return false;
  }
  if (!parser.Found(wxT_2("o"), &output_name)) {
    wxLogError(wxT_2("Output not specified."));
    return false;
  }
  return true;
}

// This implements the main function.
IMPLEMENT_APP(AnsiConvApp);
