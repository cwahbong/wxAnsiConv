#include "AnsiData.h"

#include "RawAnsi.h"

#include <wx/dcmemory.h>
#include <wx/log.h>

#include <wx/datstrm.h>
#include <wx/mstream.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include <string>
#include <utility>

using std::make_pair;
using std::pair;
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

  wxLogMessage("Get.");
  // Get colors and text.
  const vector<RawAnsiSegment>& segments = calc_segments(buf_str);
  vector<pair<int, string> > color_cmds;
  int pos = 0;
  for (vector<RawAnsiSegment>::const_iterator it = segments.begin();
       it != segments.end(); ++it) {
    if (it->escaped) {
      color_cmds.push_back(make_pair(pos, buf_str.substr(it->begin + 2, it->size - 3)));
    }
    else {
      unescaped_buf.append(buf_str, it->begin, it->size);
      pos += it->size; // TODO fix this by subtracting \r and \n
    }
  }

  // Process Text;
  TextSegment(wxString(unescaped_buf.c_str(), conv));
  wxLogMessage("Text.");

  // Assign colors
  AnsiColor color = default_color();
  pos = 0;
  size_t cidx = -1;
  for (size_t r = 0, maxr = data.size(); r < maxr; ++r) {
    for (size_t c = 0, maxc = data[r].size(); c < maxc; ++c, ++pos) {
      while (cidx != color_cmds.size() && color_cmds[cidx + 1].first <= pos) {
        ++cidx;
        wxLogMessage("CU %u %u", (unsigned) r, (unsigned) c);
        color.update(color_cmds[cidx].second);
      }
      data[r][c].color = color;
    }
    for (size_t c = data[r].size(); c < w; ++c) {
      data[r].push_back((AnsiChar){' ', color, false});
    }
    ++pos; /// XXX need to consider \r\n
  }
  wxLogMessage("Colors.");
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
      v.push_back((AnsiChar){ch, {}, false});
      if (!ch.IsAscii()) {
        v.push_back((AnsiChar){ch, {}, true});
      }
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

namespace {

void
draw_text(wxDC& dc, size_t char_size, size_t r, size_t hc, const AnsiChar& ac)
{
  wxBitmap char_bitmap(char_size, char_size);

  rgb brgb = getRgb(false, ac.color.bg);
  wxMemoryDC mdc(char_bitmap);
  wxBrush brush;
  brush.SetColour(
      brgb.r,
      brgb.g,
      brgb.b
  );
  mdc.SetBackground(brush);
  mdc.Clear();

  rgb frgb = getRgb(ac.color.bright, ac.color.fg);
  mdc.SetFont(wxFontInfo(char_size * 0.75));
  mdc.SetTextForeground(wxColour{
      frgb.r,
      frgb.g,
      frgb.b
  });
  mdc.DrawText(ac.ch, 0, 0);
  mdc.SelectObject(wxNullBitmap);

  int pr = r * char_size;
  int pc = hc * char_size / 2;
  wxRect rect{
    static_cast<int>(ac.r ? char_size / 2 : 0),
    0,
    static_cast<int>(char_size / 2),
    static_cast<int>(char_size)
  };
  dc.DrawBitmap(char_bitmap.GetSubBitmap(rect), pc, pr);
}

} // namespace

void
draw(wxDC& dc, const AnsiData& ad, size_t char_size)
{
  dc.SetBackground(*wxWHITE);
  dc.Clear();
  for (size_t r = 0, maxr = ad.Height(); r < maxr; ++r) {
    for (size_t c = 0, maxc = ad.Width(); c < maxc; ++c) {
      AnsiChar ac = ad.Get(r, c);
      draw_text(dc, char_size, r, c, ac);
    }
  }
}

wxBitmap
toBitmap(const AnsiData& ad, size_t char_size)
{
  wxBitmap bitmap((ad.Width() * char_size) / 2, ad.Height() * char_size);
  if (bitmap.IsOk()) {
    wxLogMessage("Bitmap (%d, %d)\n", bitmap.GetWidth(), bitmap.GetHeight());
  } else {
    wxLogMessage("BITMAP NOT OK\n");
  }
  wxMemoryDC dc(bitmap);
  draw(dc, ad, char_size);
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
