#include "AnsiData.h"

#include <wx/dcmemory.h>
#include <wx/sstream.h>
#include <wx/wfstream.h>

AnsiData::AnsiData(const wxString& s)
{
  // TODO
}

void
AnsiData::Draw(wxDC& dc) const
{
  // TODO
}

AnsiData
AnsiData::FromFile(wxFile& file)
{
  wxFileInputStream fis(file);
  wxStringOutputStream sos;
  fis.Read(sos);
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
