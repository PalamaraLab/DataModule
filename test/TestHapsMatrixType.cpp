// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "HapsMatrixType.hpp"

#include <catch2/catch.hpp>

#include <filesystem>
#include <string>

namespace asmc {

TEST_CASE("HapsMatrixType: test file validation", "[HapsMatrixType]") {

  // Nonexistent path
  {
    CHECK_THROWS_WITH(HapsMatrixType(FileFormat::HapsPlusSamples, DATA_MODULE_TEST_DIR "/does/not/exist"),
                      Catch::Contains("Expected path to haps/samples files, but"));
  }

  // Path exists but doesn't contain the correct files
  {
    CHECK_THROWS_WITH(HapsMatrixType(FileFormat::HapsPlusSamples, DATA_MODULE_TEST_DIR),
                      Catch::Contains("Cannot find .sample[s] file in"));
  }

  // Files exist
  CHECK_NOTHROW(HapsMatrixType(FileFormat::HapsPlusSamples, DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test"));
}

} // namespace asmc
