// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "Utils.hpp"

#include <catch2/catch.hpp>

#include <zlib.h>

#include <filesystem>
#include <string>
#include <vector>

namespace asmc {

std::vector<std::string> readLinesFromGzFile(const std::string& fileName) {

  CHECK(std::filesystem::is_regular_file(std::filesystem::path(fileName)));

  auto gzFile = gzopen(fileName.c_str(), "r");
  std::vector<std::string> lines;

  while (!gzeof(gzFile)) {
    lines.emplace_back(readNextLineFromGzip(gzFile));
  }

  gzclose(gzFile);

  return lines;
}

TEST_CASE("Util: read gz lines from basic file", "[Utils]") {

  // Basic file, newline at end
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/newline_at_end.gz");
    CHECK(lines.size() == 4);
    CHECK(lines.at(0) == "line 1");
    CHECK(lines.at(1) == "line 2");
    CHECK(lines.at(2) == "line 3");
    CHECK(lines.at(3).empty());
  }

  // Basic file, no newline at end
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/no_newline_at_end.gz");
    CHECK(lines.size() == 3);
    CHECK(lines.at(0) == "line 1");
    CHECK(lines.at(1) == "line 2");
    CHECK(lines.at(2) == "line 3");
  }
}

TEST_CASE("Util: read gz lines of length 1 less than buffer", "[Utils]") {

  // Line with 511 z characters and newline
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/511z_newline.gz");
    CHECK(lines.size() == 3);
    CHECK(lines.at(0) == "Next line has 511 z:");
    CHECK(lines.at(1) == std::string(511, 'z'));
    CHECK(lines.at(2).empty());
  }

  // Line with 511 z characters and no newline
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/511z_no_newline.gz");
    CHECK(lines.size() == 2);
    CHECK(lines.at(0) == "Next line has 511 z:");
    CHECK(lines.at(1) == std::string(511, 'z'));
  }
}

TEST_CASE("Util: read gz lines of length same as buffer", "[Utils]") {

  // Line with 512 z characters and newline
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/512z_newline.gz");
    CHECK(lines.size() == 3);
    CHECK(lines.at(0) == "Next line has 512 z:");
    CHECK(lines.at(1) == std::string(512, 'z'));
    CHECK(lines.at(2).empty());
  }

  // Line with 512 z characters and no newline
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/512z_no_newline.gz");
    CHECK(lines.size() == 2);
    CHECK(lines.at(0) == "Next line has 512 z:");
    CHECK(lines.at(1) == std::string(512, 'z'));
  }
}

TEST_CASE("Util: read gz lines of length 1 more than buffer", "[Utils]") {

  // Line with 513 z characters and newline
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/513z_newline.gz");
    CHECK(lines.size() == 3);
    CHECK(lines.at(0) == "Next line has 513 z:");
    CHECK(lines.at(1) == std::string(513, 'z'));
    CHECK(lines.at(2).empty());
  }

  // Line with 513 z characters and no newline
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/513z_no_newline.gz");
    CHECK(lines.size() == 2);
    CHECK(lines.at(0) == "Next line has 513 z:");
    CHECK(lines.at(1) == std::string(513, 'z'));
  }
}

TEST_CASE("Util: read gz lines that are very long", "[Utils]") {

  // Line with 8192 z characters and newline
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/very_long_line_newline.gz");
    CHECK(lines.size() == 3);
    CHECK(lines.at(0) == "Next line has 8192 z:");
    CHECK(lines.at(1) == std::string(8192, 'z'));
    CHECK(lines.at(2).empty());
  }

  // Line with 8192 z characters and no newline
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/very_long_line_no_newline.gz");
    CHECK(lines.size() == 2);
    CHECK(lines.at(0) == "Next line has 8192 z:");
    CHECK(lines.at(1) == std::string(8192, 'z'));
  }
}

TEST_CASE("Util: read gz lines with miscellaneous checks", "[Utils]") {

  // Completely empty file
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/empty_file.gz");
    CHECK(lines.size() == 1);
    CHECK(lines.at(0).empty());
  }

  // File containing only newline characters
  {
    const auto lines = readLinesFromGzFile(DATA_MODULE_TEST_DIR "/data/util/only_newline_characters.gz");
    CHECK(lines.size() == 3);
    CHECK(lines.at(0).empty());
    CHECK(lines.at(1).empty());
    CHECK(lines.at(2).empty());
  }
}

TEST_CASE("Util: test getFileInDirWithExt", "[Utils]") {

  const std::string pathPrefix = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test";

  // No matching extensions
  {
    const auto path = getFileInDirWithExt(pathPrefix, {".not_exist"});
    CHECK(path == fs::path{});
  }

  // No extensions supplied
  {
    const auto path = getFileInDirWithExt(pathPrefix, {});
    CHECK(path == fs::path{});
  }

  // Valid extension: .hap found
  {
    const auto path = getFileInDirWithExt(pathPrefix, {".hap", ".samples"});
    CHECK(path == fs::path{pathPrefix + ".hap"});
  }

  // Valid extension: .samples found
  {
    const auto path = getFileInDirWithExt(pathPrefix, {".samples", ".hap"});
    CHECK(path == fs::path{pathPrefix + ".samples"});
  }
}

} // namespace asmc
