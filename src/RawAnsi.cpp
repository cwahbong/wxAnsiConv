#include "RawAnsi.h"

using std::string;
using std::vector;

namespace {

// Returns the size of string instead of string::npos when pattern is not
// found.
size_t
find_or_end(const string& s, const string& pattern, size_t pos = 0)
{
  size_t result = s.find(pattern, pos);
  return result == string::npos ? s.size() : result;
}

RawAnsiSegment
scan(const string& data, size_t start)
{
  size_t next_escaped_start = find_or_end(data, "\033[", start);
  return next_escaped_start == start ?
      (RawAnsiSegment) {start, find_or_end(data, "m", start) - start, true} :
      (RawAnsiSegment) {start, next_escaped_start - start, false};
}

} // namespace

vector<RawAnsiSegment>
calc_segments(const string& data)
{
  vector<RawAnsiSegment> result;
  size_t i = 0;
  while (i < data.size()) {
    RawAnsiSegment ras = scan(data, i);
    i += ras.size;
    result.push_back(ras);
  }
  return result;
}
