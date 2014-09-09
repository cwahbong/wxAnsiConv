#include <wx/wx.h>
#include <wx/dc.h>
#include <wx/file.h>

#include <vector>

struct AnsiChar {
  int fg, bg;
  wxChar c;
};

class AnsiData {
public:
  AnsiData(const wxString&);
  size_t Width() const;
  size_t Height() const;
  AnsiChar Get(size_t r_pos, size_t c_pos) const;
  void Draw(wxDC&) const;

  static AnsiData FromFile(wxFile&);

private:
  size_t w;
  std::vector<std::vector<AnsiChar> > data;
};

wxBitmap toBitmap(const AnsiData&);
wxImage toImage(const AnsiData&);
