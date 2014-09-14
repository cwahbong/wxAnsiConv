#include "AnsiColor.h"

#include <wx/wx.h>
#include <wx/dc.h>
#include <wx/file.h>

#include <vector>

struct AnsiChar {
  wxUniChar ch;
  AnsiColor color;
  bool r;
};

class AnsiData {
public:
  AnsiData(const void* buffer, size_t len, const wxString& encoding);
  size_t Width() const;
  size_t Height() const;
  AnsiChar Get(size_t r_pos, size_t c_pos) const;

  static AnsiData FromFile(wxInputStream&, const wxString&);
  static AnsiData FromFile(const wxString&, const wxString&);

private:
  void TextSegment(const wxString&);

  size_t w;

  AnsiColor color;
  std::vector<std::vector<AnsiChar> > data;
  std::vector<AnsiChar> v;
};

void draw(wxDC&, const AnsiData&, size_t);

wxBitmap toBitmap(const AnsiData&, size_t);
wxImage toImage(const AnsiData&, size_t);
