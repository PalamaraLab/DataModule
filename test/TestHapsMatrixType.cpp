// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "HapsMatrixType.hpp"

#include <catch2/catch.hpp>

#include <string>

namespace asmc {

TEST_CASE("HapsMatrixType: test CreateFromHapsPlusSamples", "[HapsMatrixType]") {

  std::string goodHapsFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.hap";
  std::string goodSamplesFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.samples";
  std::string goodMapFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.map";

  std::string badHapsFile = DATA_MODULE_TEST_DIR "/does/not/exist.haps";
  std::string badSamplesFile = DATA_MODULE_TEST_DIR "/does/not/exist.samples";
  std::string badMapFile = DATA_MODULE_TEST_DIR "/data"; // directory, not file

  // First path is bad
  CHECK_THROWS_WITH(HapsMatrixType::CreateFromHapsPlusSamples(badHapsFile, goodSamplesFile, goodMapFile),
                    Catch::StartsWith("Expected .hap[s][.gz] file, but got "));

  // Second path is bad
  CHECK_THROWS_WITH(HapsMatrixType::CreateFromHapsPlusSamples(goodHapsFile, badSamplesFile, goodMapFile),
                    Catch::StartsWith("Expected .sample[s] file, but got "));

  // Third path is bad
  CHECK_THROWS_WITH(HapsMatrixType::CreateFromHapsPlusSamples(goodHapsFile, goodSamplesFile, badMapFile),
                    Catch::StartsWith("Expected .map file, but got "));

  // Constructs fine
  CHECK_NOTHROW(HapsMatrixType::CreateFromHapsPlusSamples(goodHapsFile, goodSamplesFile, goodMapFile));
}

} // namespace asmc
