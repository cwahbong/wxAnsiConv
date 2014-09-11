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
  const vector<RawAnsiSegment>& segments = calc_segments(buf_str);
  for (vector<RawAnsiSegment>::const_iterator it = segments.begin();
       it != segments.end(); ++it) {
    if (it->escaped) {
      wxMemoryInputStream mis(buf_str.c_str() + it->begin + 2, it->size - 3);
      EscapeSegment(mis, conv);
    }
    else {
      wxMemoryInputStream mis(buf_str.c_str() + it->begin, it->size);
      TextSegment(mis, conv);
    }
  }
}

void
AnsiData::EscapeSegment(wxInputStream& is, wxMBConv& conv)
{
  // Escaped codes are separated by semicolons.
  wxTextInputStream tis(is, ";", conv);
  // TODO parse the color code
}

void
AnsiData::TextSegment(wxInputStream& is, wxMBConv& conv)
{
  wxTextInputStream tis(is, "", conv);
  while (true) {
    wxChar ch = tis.GetChar();
    if (ch == 0) {
      break;
    }
    if (ch == '\n') {
      if (v.size() > w) {
        w = v.size();
      }
      data.push_back(v);
      v.clear();
    } else {
      v.push_back((AnsiChar){color, ch});
    }
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
  for (size_t r = 0, maxr = Height(); r < maxr; ++r) {
    const vector<AnsiChar>& row = data[r];
    for (size_t c = 0, maxc = Width(); c < maxc; ++c) {
      AnsiChar ac = c < row.size() ? row[c] : AnsiChar();
      dc.DrawText(ac.ch, c * char_size, r * char_size);
    }
  }
}

AnsiData
AnsiData::FromFile(wxInputStream& is, const wxString& encoding)
{
  wxMemoryOutputStream mos;
  mos.Write(is);
  // wxLogMessage("Read %u.", (unsigned) fis.LastRead());
  // wxLogMessage("Write %u.", (unsigned) mos.LastWrite());
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
  dc.SetBackground(*wxWHITE_BRUSH);
  dc.Clear();
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
