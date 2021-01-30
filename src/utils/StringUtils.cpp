// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "StringUtils.hpp"

#include <string>
#include <string_view>
#include <vector>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>

namespace asmc {

std::vector<std::string> splitTextByDelimiter(std::string_view text, std::string_view del) {
  return text | ranges::views::split(del) | ranges::to<std::vector<std::string>>();
}

std::string stripBack(std::string s) {
  while (!s.empty() && (s.back() == '\n' || s.back() == ' ' || s.back() == '\t' || s.back() == '\r')) {
    s.pop_back();
  }
  return s;
}

} // namespace asmc
