#include <wx/wx.h>
#include <wx/cmdline.h>
#include <wx/file.h>

#include "AnsiData.h"

class AnsiConvApp: public wxApp
{
public:
  virtual void OnInitCmdLine(wxCmdLineParser &);
  virtual bool OnCmdLineParsed(wxCmdLineParser &);
  virtual int OnRun();

private:
  wxString input_name;
  wxString output_name;
  wxString encoding;
};

namespace {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
const wxCmdLineEntryDesc cmdLineDesc[] = {
  {
    wxCMD_LINE_OPTION,
    "i",
    "input",
    "The input ansi art file.",
    wxCMD_LINE_VAL_STRING,
    wxCMD_LINE_OPTION_MANDATORY
  },
  {
    wxCMD_LINE_OPTION,
    "o",
    "output",
    "The output picture.",
    wxCMD_LINE_VAL_STRING,
    wxCMD_LINE_OPTION_MANDATORY,
  },
  {
    wxCMD_LINE_OPTION,
    "e",
    "encoding",
    "The encding of input.",
    wxCMD_LINE_VAL_STRING,
  },
  {wxCMD_LINE_NONE},
};
#pragma GCC diagnostic pop

} // namespace

void
AnsiConvApp::OnInitCmdLine(wxCmdLineParser &parser)
{
  parser.SetDesc(cmdLineDesc);
  wxAppConsole::OnInitCmdLine(parser);
}

bool
AnsiConvApp::OnCmdLineParsed(wxCmdLineParser &parser)
{
  parser.Found("i", &input_name);
  parser.Found("o", &output_name);
  parser.Found("e", &encoding);
  return wxAppConsole::OnCmdLineParsed(parser);
}

int
AnsiConvApp::OnRun()
{
  wxLog::SetActiveTarget(new wxLogStderr);
  wxInitAllImageHandlers();
  const AnsiData& ad = AnsiData::FromFile(input_name, encoding);
  wxImage image = toImage(ad, 12);
  image.SaveFile(output_name, "image/png");
  return 0;
}

// This implements the main function.
IMPLEMENT_APP(AnsiConvApp);
