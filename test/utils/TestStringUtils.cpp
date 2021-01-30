// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "utils/StringUtils.hpp"

#include <catch2/catch.hpp>

#include <string>
#include <vector>

namespace asmc {

TEST_CASE("utils/StringUtils: test splitTextByDelimiter", "[utils/StringUtils]") {

  // Test space delimited
  {
    const std::string text = "a b c def";
    const auto splitText = splitTextByDelimiter(text, " ");
    CHECK(splitText == std::vector<std::string>{"a", "b", "c", "def"});
  }

  // Test comma delimited
  {
    const std::string text = "a b c, def";
    const auto splitText = splitTextByDelimiter(text, ",");
    CHECK(splitText == std::vector<std::string>{"a b c", " def"});
  }

  // Test multi-character delimited
  {
    const std::string text = "abc>=def>=ghi";
    const auto splitText = splitTextByDelimiter(text, ">=");
    CHECK(splitText == std::vector<std::string>{"abc", "def", "ghi"});
  }

  // Test empty string
  {
    const std::string text;
    CHECK(text.empty());
    const auto splitText = splitTextByDelimiter(text, " ");
    CHECK(splitText.empty());
  }
}

TEST_CASE("utils/StringUtils: test stripBack", "[utils/StringUtils]") {

  // Test string with no whitespace
  {
    const std::string text = "abc";
    CHECK(text == stripBack(text));
  }

  // Test string with only whitespace
  {
    const std::string text = "\t\n\r ";
    CHECK(stripBack(text).empty());
  }

  // Test regular string
  {
    const std::string text = "abc\t\n\r ";
    CHECK(stripBack(text) == "abc");
  }
}

} // namespace asmc
