// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "HapsMatrixType.hpp"

#include <catch2/catch.hpp>

#include <string>

namespace asmc {

TEST_CASE("HapsMatrixType: test createFromHapsPlusSamples", "[HapsMatrixType]") {

  std::string goodHapsFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.hap";
  std::string goodSamplesFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.samples";
  std::string goodMapFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.map";

  std::string badHapsFile = DATA_MODULE_TEST_DIR "/does/not/exist.haps";
  std::string badSamplesFile = DATA_MODULE_TEST_DIR "/does/not/exist.samples";
  std::string badMapFile = DATA_MODULE_TEST_DIR "/data"; // directory, not file

  // First path is bad
  CHECK_THROWS_WITH(HapsMatrixType::createFromHapsPlusSamples(badHapsFile, goodSamplesFile, goodMapFile),
                    Catch::StartsWith("Expected .hap[s][.gz] file, but got "));

  // Second path is bad
  CHECK_THROWS_WITH(HapsMatrixType::createFromHapsPlusSamples(goodHapsFile, badSamplesFile, goodMapFile),
                    Catch::StartsWith("Expected .sample[s] file, but got "));

  // Third path is bad
  CHECK_THROWS_WITH(HapsMatrixType::createFromHapsPlusSamples(goodHapsFile, goodSamplesFile, badMapFile),
                    Catch::StartsWith("Expected .map file, but got "));

  // Constructs fine
  CHECK_NOTHROW(HapsMatrixType::createFromHapsPlusSamples(goodHapsFile, goodSamplesFile, goodMapFile));
  const auto hapsMatrix = HapsMatrixType::createFromHapsPlusSamples(goodHapsFile, goodSamplesFile, goodMapFile);
  CHECK(hapsMatrix.getNumIndividuals() == 3ul);
  CHECK(hapsMatrix.getNumSites() == 4ul);

  {
    const std::vector<unsigned long>& physicalPositions = hapsMatrix.getPhysicalPositions();
    CHECK(physicalPositions.size() == 4ul);
    CHECK(physicalPositions.at(0) == 2022ul);
    CHECK(physicalPositions.at(1) == 2082ul);
    CHECK(physicalPositions.at(2) == 2690ul);
    CHECK(physicalPositions.at(3) == 2867ul);
  }

  {
    const std::vector<double>& geneticPositions = hapsMatrix.getGeneticPositions();
    CHECK(geneticPositions.size() == 4ul);
    CHECK(geneticPositions.at(0) == Approx(0.00095114141));
    CHECK(geneticPositions.at(1) == Approx(0.00097937905));
    CHECK(geneticPositions.at(2) == Approx(0.001256979));
    CHECK(geneticPositions.at(3) == Approx(0.0013181955));
  }

}

} // namespace asmc
