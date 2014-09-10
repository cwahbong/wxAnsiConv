#include <string>
#include <vector>

struct RawAnsiSegment {
  size_t begin;
  size_t size;
  bool escaped;
};

std::vector<RawAnsiSegment> calc_segments(const std::string&);
