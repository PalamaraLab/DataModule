// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "Utils.hpp"

#include <array>
#include <string_view>

namespace asmc {

std::string readNextLineFromGzip(gzFile& gzFileHandle) {

  std::array<char, 512> buffer = {};
  std::string line;

  char* successful_read = nullptr;
  do {
    successful_read = gzgets(gzFileHandle, buffer.data(), static_cast<int>(buffer.size()));

    if (successful_read != Z_NULL) {
      line += buffer.data();
    }

  } while (!line.empty() && line.back() != '\n' && !gzeof(gzFileHandle));

  if (!line.empty() && line.back() == '\n') {
    line.pop_back();
  }

  return line;
}

} // namespace asmc
