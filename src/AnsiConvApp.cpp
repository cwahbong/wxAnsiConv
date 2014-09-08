#include <wx/wx.h>
#include <wx/cmdline.h>

class AnsiConvApp: public wxApp
{
public:
  virtual void OnInitCmdLine(wxCmdLineParser &);
  virtual bool OnCmdLineParsed(wxCmdLineParser &);
  virtual int OnRun();

private:
  wxString input_name;
  wxString output_name;
};

void
AnsiConvApp::OnInitCmdLine(wxCmdLineParser &parser)
{
  parser.AddOption(wxT_2("i"),
                   wxT_2("input"),
                   wxT_2("The input ansi art file."),
                   wxCMD_LINE_VAL_STRING,
                   wxCMD_LINE_OPTION_MANDATORY);
  parser.AddOption(wxT_2("o"),
                   wxT_2("output"),
                   wxT_2("The output picture."),
                   wxCMD_LINE_VAL_STRING,
                   wxCMD_LINE_OPTION_MANDATORY);
}

bool
AnsiConvApp::OnCmdLineParsed(wxCmdLineParser &parser)
{
  parser.Found(wxT_2("i"), &input_name);
  parser.Found(wxT_2("o"), &output_name);
  return wxApp::OnCmdLineParsed(parser);
}

int
AnsiConvApp::OnRun()
{
  // TODO read input
  // TODO generate wxImage
  // TODO write image
  return 0;
}

// This implements the main function.
IMPLEMENT_APP(AnsiConvApp);
