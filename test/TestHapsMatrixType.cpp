// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "HapsMatrixType.hpp"

#include <catch2/catch.hpp>

#include <cstdint>
#include <string>

#include <fmt/core.h>
#include <fmt/ostream.h>

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

  const auto& data = hapsMatrix.getData();
  CHECK(data.rows() == static_cast<index_t>(4l));
  CHECK(data.cols() == static_cast<index_t>(6l));
  CHECK(data(0,0));
  CHECK(!data(0,1));
  CHECK(!data(0,2));
  CHECK(!data(0,3));
  CHECK(!data(0,4));
  CHECK(!data(0,5));
  CHECK(!data(2,0));
  CHECK(!data(2,1));
  CHECK(!data(2,2));
  CHECK(data(2,3));
  CHECK(!data(2,4));
  CHECK(!data(2,5));

  Eigen::Matrix<uint8_t, 1, 6> row1;
  row1 << 0, 0, 0, 0, 0, 0;
  CHECK(row1 == hapsMatrix.getSite(1));

  Eigen::Matrix<uint8_t, 4, 1> col3;
  col3 << 0, 0, 1, 0;
  CHECK(col3 == hapsMatrix.getHap(3));
}

TEST_CASE("HapsMatrixType: test readSamplesFile", "[HapsMatrixType]") {

  std::string goodHapsFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.hap";
  std::string goodMapFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.map";

  std::string badSamples1 = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/bad_header_1.samples";
  std::string badSamples2 = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/bad_header_2.samples";

  CHECK_THROWS_WITH(HapsMatrixType::createFromHapsPlusSamples(goodHapsFile, badSamples1, goodMapFile),
                    Catch::StartsWith("Expected fist row of .samples file "));

  CHECK_THROWS_WITH(HapsMatrixType::createFromHapsPlusSamples(goodHapsFile, badSamples2, goodMapFile),
                    Catch::StartsWith("Expected second row of .samples file "));
}

TEST_CASE("HapsMatrixType: test readHapsFile", "[HapsMatrixType]") {

  std::string hapsTooFewRows = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/too_few_rows.hap";
  std::string hapsTooManyRows = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/too_many_rows.hap";
  std::string hapsNotBoolean = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/not_boolean.hap";

  std::string goodMapFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.map";
  std::string goodSamplesFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/test.samples";

  CHECK_THROWS_WITH(HapsMatrixType::createFromHapsPlusSamples(hapsTooFewRows, goodSamplesFile, goodMapFile),
                    Catch::StartsWith("Error on line 3 of"));
  CHECK_THROWS_WITH(HapsMatrixType::createFromHapsPlusSamples(hapsTooManyRows, goodSamplesFile, goodMapFile),
                    Catch::Contains("to contain 4 lines, but found 5"));
  CHECK_THROWS_WITH(HapsMatrixType::createFromHapsPlusSamples(hapsNotBoolean, goodSamplesFile, goodMapFile),
                    Catch::Contains("but column 9 was \"invalid\""));
}

TEST_CASE("HapsMatrixType: test (small) real example", "[HapsMatrixType]") {

  std::string hapsFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/real_example.haps.gz";
  std::string samplesFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/real_example.sample.gz";
  std::string mapFile = DATA_MODULE_TEST_DIR "/data/haps_plus_samples/real_example.map.gz";

  auto hapsMatrix = HapsMatrixType::createFromHapsPlusSamples(hapsFile, samplesFile, mapFile);
  CHECK(hapsMatrix.getData().rows() == static_cast<index_t>(102l));
  CHECK(hapsMatrix.getData().cols() == static_cast<index_t>(100l));
  CHECK(hapsMatrix.getNumSites() == 102ul);
  CHECK(hapsMatrix.getNumIndividuals() == 50ul);
  CHECK(hapsMatrix.getNumHaps() == 100ul);

  // Test counts
  {
    CHECK(hapsMatrix.getMinorAlleleCount(0ul) == 1ul);
    CHECK(hapsMatrix.getDerivedAlleleCount(0ul) == 1ul);

    CHECK(hapsMatrix.getMinorAlleleCount(8ul) == 29ul);
    CHECK(hapsMatrix.getDerivedAlleleCount(8ul) == 71ul);

    cvec_ul_t minorAlleleCounts = hapsMatrix.getMinorAlleleCounts();
    CHECK(minorAlleleCounts(0) == 1ul);
    CHECK(minorAlleleCounts(8) == 29ul);

    cvec_ul_t derivedAlleleCounts = hapsMatrix.getDerivedAlleleCounts();
    CHECK(derivedAlleleCounts(0) == 1ul);
    CHECK(derivedAlleleCounts(8) == 71ul);
  }

  // Test frequencies
  {
    CHECK(hapsMatrix.getMinorAlleleFrequency(0ul) == Approx(1.0 / 100.0));
    CHECK(hapsMatrix.getDerivedAlleleFrequency(0ul) == Approx(1.0 / 100.0));

    CHECK(hapsMatrix.getMinorAlleleFrequency(8ul) == Approx(29.0 / 100.0));
    CHECK(hapsMatrix.getDerivedAlleleFrequency(8ul) == Approx(71.0 / 100.0));

    cvec_dbl_t minorAlleleFrequencies = hapsMatrix.getMinorAlleleFrequencies();
    CHECK(minorAlleleFrequencies(0) == Approx(1.0 / 100.0));
    CHECK(minorAlleleFrequencies(8) == Approx(29.0 / 100.0));

    cvec_dbl_t derivedAlleleFrequencies = hapsMatrix.getDerivedAlleleFrequencies();
    CHECK(derivedAlleleFrequencies(0) == Approx(1.0 / 100.0));
    CHECK(derivedAlleleFrequencies(8) == Approx(71.0 / 100.0));
  }
}

} // namespace asmc
