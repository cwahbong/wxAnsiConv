#include "AnsiColor.h"

#include <iostream>

using std::cerr;
using std::string;

namespace {

const rgb colors_bright[] = {
  {0x55, 0x55, 0x55}, // dark gray
  {0xFF, 0x55, 0x55}, // light red
  {0x55, 0xFF, 0x55}, // light green
  {0xFF, 0xFF, 0x55}, // yellow
  {0x55, 0x55, 0xFF}, // light blue
  {0xFF, 0x55, 0xFF}, // light purple
  {0x55, 0xFF, 0xFF}, // light cyan
  {0xFF, 0xFF, 0xFF}, // white
};

const rgb colors_dark[] = {
  {0x00, 0x00, 0x00}, // black
  {0xAA, 0x00, 0x00}, // red
  {0x00, 0xAA, 0x00}, // green
  {0xAA, 0x55, 0x00}, // brown
  {0x00, 0x00, 0xAA}, // blue
  {0xAA, 0x00, 0xAA}, // purple
  {0x00, 0xAA, 0xAA}, // cyan
  {0xAA, 0xAA, 0xAA}, // light gray
};

} // namespace

void
AnsiColor::update(const string& s)
{
  cerr << "COLOR: " << s << "\n";
  if (s.empty()) {
    *this = default_color();
    cerr << "Reset parsed.\n";
    return;
  }
  size_t from = 0;
  while (from < s.size()) {
    size_t to = s.find_first_of(";", from);
    if (to == string::npos) {
      to = s.size();
    }
    int n = 0;
    if (to - from > 0) {
      n = stoi(s.substr(from, to - from));
    }
    if (n == 0) {
      *this = default_color();
      cerr << "Reset parsed.\n";
    }
    else if (n == 1) {
      bright = true;
      cerr << "Bright parsed.\n";
    }
    else if (n == 4) {
      underline = true;
      cerr << "Underline parsed.\n";
    }
    else if (n == 5) {
      blink = true;
      cerr << "Blink parsed.\n";
    }
    else if (n == 7) {
      inverse = true;
      cerr << "Inverse parsed.\n";
    }
    else if (n >= 30 && n <= 37) {
      fg = n % 10;
    }
    else if (n >= 40 && n <= 47) {
      bg = n % 10;
    }
    else {
      cerr << "AnsiColor.cpp parse(): n = " << n << "\n";
    }
    from = to + 1;
  }
}

rgb
getRgb(bool bright, int color)
{
  return bright ? colors_bright[color] : colors_dark[color];
}

AnsiColor
default_color()
{
  return AnsiColor{
    false,
    false,
    false,
    false,
    7, 0,
  };
}
