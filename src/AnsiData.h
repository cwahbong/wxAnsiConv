#include <wx/wx.h>
#include <wx/dc.h>
#include <wx/file.h>

#include <vector>

struct AnsiColor {
  int fg, bg;
};

struct AnsiChar {
  AnsiColor color;
  wxUniChar ch;
};

class AnsiData {
public:
  AnsiData(const void* buffer, size_t len, const wxString& encoding);
  size_t Width() const;
  size_t Height() const;
  AnsiChar Get(size_t r_pos, size_t c_pos) const;

  void Draw(wxDC&, size_t) const;

  static AnsiData FromFile(wxInputStream&, const wxString&);
  static AnsiData FromFile(const wxString&, const wxString&);

private:
  void EscapeSegment(wxInputStream&, wxMBConv&);
  void TextSegment(const wxString&);

  size_t w;

  AnsiColor color;
  std::vector<std::vector<AnsiChar> > data;
  std::vector<AnsiChar> v;
};

wxBitmap toBitmap(const AnsiData&, size_t);
wxImage toImage(const AnsiData&, size_t);
