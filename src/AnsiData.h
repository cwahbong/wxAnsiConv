#include <wx/wx.h>
#include <wx/dc.h>
#include <wx/file.h>

class AnsiData {
public:
  AnsiData(const wxString&);
  void Draw(wxDC&) const;

  static AnsiData FromFile(wxFile&);
};

wxBitmap toBitmap(const AnsiData&);
wxImage toImage(const AnsiData&);
