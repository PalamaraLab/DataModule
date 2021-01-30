// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "Utils.hpp"

#include <array>
#include <string_view>

#include <fmt/core.h>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>

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

  while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) {
    line.pop_back();
  }

  return line;
}

std::vector<std::string> splitTextByDelimiter(std::string_view text, std::string_view del) {
  return text | ranges::views::split(del) | ranges::to<std::vector<std::string>>();
}

} // namespace asmc
