#include <string>

struct rgb {
  int r, g, b;
};

struct AnsiColor {
  bool bright;
  bool underline;
  bool blink;
  bool inverse;
  bool bfg, bbg;
  rgb fg, bg;
};

AnsiColor default_color();
AnsiColor parse(const std::string&);
