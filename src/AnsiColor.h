#include <string>

struct rgb {
  unsigned char r, g, b;
};

struct AnsiColor {
  bool bright;
  bool underline;
  bool blink;
  bool inverse;
  int fg, bg;

  void update(const std::string&);
};

rgb getRgb(bool bright, int color);
AnsiColor default_color();
