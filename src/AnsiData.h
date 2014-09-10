#include <wx/wx.h>
#include <wx/dc.h>
#include <wx/file.h>

#include <vector>

struct AnsiColor {
  int fg, bg;
};

struct AnsiChar {
  AnsiColor color;
  wxChar ch;
};

class AnsiData {
public:
  AnsiData(const void* buffer, size_t len, const wxString& encoding);
  size_t Width() const;
  size_t Height() const;
  AnsiChar Get(size_t r_pos, size_t c_pos) const;

  void Draw(wxDC&) const;

  static AnsiData FromFile(wxInputStream&, const wxString&);
  static AnsiData FromFile(const wxString&, const wxString&);

private:
  void EscapeSegment(wxInputStream&, wxMBConv&);
  void TextSegment(wxInputStream&, wxMBConv&);

  size_t w;

  AnsiColor color;
  std::vector<std::vector<AnsiChar> > data;
  std::vector<AnsiChar> v;
};

wxBitmap toBitmap(const AnsiData&);
wxImage toImage(const AnsiData&);
