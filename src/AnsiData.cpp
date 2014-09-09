#include "AnsiData.h"

#include <wx/dcmemory.h>
#include <wx/log.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

using std::vector;

AnsiData::AnsiData(const wxString& s)
: w(0)
{
  wxStringInputStream sis(s);
  wxTextInputStream tis(sis);
  int fg = 0, bg = 0;
  wxLogMessage("Constructing %s\n", s.wx_str());
  while(!sis.Eof()) {
    wxString line;
    tis >> line;
    // wxLogMessage("Line with len %d\n", int(line.Len()));
    vector<AnsiChar> v;
    for (size_t i = 0, maxi = line.Len(); i < maxi; ++i) {
      wxLogMessage("%c\n", line[i]);

      if (line[i] == '[') {
        // Change color mode
      }
      v.push_back((AnsiChar){fg, bg, line[i]});
    }
    if(v.size() > w) {
      w = v.size();
    }
    data.push_back(v);
  }
}

size_t
AnsiData::Width() const
{
  return w;
}

size_t
AnsiData::Height() const
{
  return data.size();
}

AnsiChar
AnsiData::Get(size_t r_pos, size_t c_pos) const
{
  return data[r_pos][c_pos];
}

void
AnsiData::Draw(wxDC& dc) const
{
  // for () {
  //   for () {
  //     data[i][j]; // TODO draw this character
  //   }
  // }
}

AnsiData
AnsiData::FromFile(wxFile& file)
{
  wxFileInputStream fis(file);
  wxStringOutputStream sos;
  fis.Read(sos);
  wxLogMessage("Read %u.", (unsigned) fis.LastRead());
  wxLogMessage("Write %u.", (unsigned) sos.LastWrite());
  return AnsiData(sos.GetString());
}

wxBitmap
toBitmap(const AnsiData& ad)
{
  wxBitmap bitmap;
  wxMemoryDC dc;
  dc.SelectObject(bitmap);
  ad.Draw(dc);
  return bitmap;
}

wxImage
toImage(const AnsiData& ad)
{
  return toBitmap(ad).ConvertToImage();
}
