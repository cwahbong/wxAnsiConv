#include "AnsiData.h"

#include "RawAnsi.h"

#include <wx/dcmemory.h>
#include <wx/log.h>

#include <wx/datstrm.h>
#include <wx/mstream.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include <string>

using std::string;
using std::vector;

AnsiData::AnsiData(const void* buffer, size_t len, const wxString& encoding)
: w(0)
{
  wxCSConv conv(encoding);
  if (!conv.IsOk()) {
    wxLogMessage("Encoding not supported. Will fallback to ISO 8859-1 instead.");
  }
  string buf_str(static_cast<const char*>(buffer), len);
  string unescaped_buf;
  const vector<RawAnsiSegment>& segments = calc_segments(buf_str);
  for (vector<RawAnsiSegment>::const_iterator it = segments.begin();
       it != segments.end(); ++it) {
    if (it->escaped) {
      wxString wxstr(buf_str.c_str() + it->begin + 2, it->size - 3);
      wxLogMessage("ESC %u %s", (unsigned) it->size, wxstr.wx_str());
      wxMemoryInputStream mis(buf_str.c_str() + it->begin + 2, it->size - 3);
      EscapeSegment(mis, wxConvISO8859_1);
    }
    else {
      unescaped_buf.append(buf_str, it->begin, it->size);
      // wxString wxstr(buf_str.c_str() + it->begin, conv, it->size);
      // TextSegment(wxstr);
    }
  }
  TextSegment(wxString(unescaped_buf.c_str(), conv));
}

void
AnsiData::EscapeSegment(wxInputStream& is, wxMBConv& conv)
{
  // Escaped codes are separated by semicolons.
  wxTextInputStream tis(is, ";", conv);
  // TODO parse the color code
}

void
AnsiData::TextSegment(const wxString& wxstr)
{
  for (size_t i = 0, maxi = wxstr.length(); i < maxi; ++i) {
    wxUniChar ch = wxstr[i];
    if (ch == '\n') {
      if (v.size() > w) {
        w = v.size();
      }
      data.push_back(v);
      v.clear();
    } else if (ch != '\r'){
      v.push_back((AnsiChar){color, ch});
    }
  }
  if (!v.empty()) {
    data.push_back(v);
    v.clear();
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
AnsiData::Draw(wxDC& dc, size_t char_size) const
{
  dc.SetFont(wxFontInfo(char_size * 0.75).Family(wxFONTFAMILY_ROMAN));
  dc.SetTextForeground(*wxBLACK);
  dc.SetTextBackground(*wxWHITE);
  dc.SetBackground(*wxWHITE);
  dc.Clear();
  for (size_t r = 0, maxr = Height(); r < maxr; ++r) {
    const vector<AnsiChar>& row = data[r];
    int rw = 0;
    for (size_t c = 0, maxc = Width(); c < maxc; ++c) {
      if (c < row.size()) {
        AnsiChar ac = row[c];
        wxCoord w, h;
        dc.GetTextExtent(ac.ch, &w, &h);
        wxLogMessage("WH %d %d", w, h);
        rw += ac.ch >= 128 ? char_size : char_size / 2;
        dc.DrawText(ac.ch, c * char_size, r * char_size);
      }
    }
  }
}

AnsiData
AnsiData::FromFile(wxInputStream& is, const wxString& encoding)
{
  wxMemoryOutputStream mos;
  mos.Write(is);
  const wxStreamBuffer* buffer = mos.GetOutputStreamBuffer();
  return AnsiData(buffer->GetBufferStart(),
                  buffer->GetBufferSize(),
                  encoding);
}


// wxStringOutputStream sos(NULL, wxConvISO8859_1);
AnsiData
AnsiData::FromFile(const wxString& name, const wxString& encoding)
{
  wxFile file(name);
  wxFileInputStream fis(file);
  if (!fis.IsOk()) {
    wxLogMessage("Error occured when opening file.");
  }
  wxLogMessage("File opened.");
  return AnsiData::FromFile(fis, encoding);
}

wxBitmap
toBitmap(const AnsiData& ad, size_t char_size)
{
  wxBitmap bitmap(ad.Width() * char_size, ad.Height() * char_size);
  if (bitmap.IsOk()) {
    wxLogMessage("Bitmap (%d, %d)\n", bitmap.GetWidth(), bitmap.GetHeight());
  } else {
    wxLogMessage("BITMAP NOT OK\n");
  }
  wxMemoryDC dc(bitmap);
  ad.Draw(dc, char_size);
  dc.SelectObject(wxNullBitmap);
  wxLogMessage("Drawed.");
  return bitmap;
}

wxImage
toImage(const AnsiData& ad, size_t char_size)
{
  wxBitmap bitmap = toBitmap(ad, char_size);
  return bitmap.ConvertToImage();
}
