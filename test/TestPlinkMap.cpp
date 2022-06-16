// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "PlinkMap.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <cstdint>
#include <iostream>
#include <string>

#include <fmt/core.h>

using Catch::Matchers::ContainsSubstring;
using Catch::Matchers::EndsWith;
using Catch::Matchers::StartsWith;

namespace asmc {

TEST_CASE("PlinkMap: test exceptions", "[PlinkMap]") {

  // Map file does not exist
  std::string nonexistentFile = DATA_MODULE_TEST_DIR "/does/not/exist.map";
  CHECK_THROWS_WITH(PlinkMap(nonexistentFile), StartsWith("Error: PLINK map file"));
  CHECK_THROWS_WITH(PlinkMap(nonexistentFile), EndsWith("does not exist\n"));

  // Map file is not tab-separated
  std::string spaceSepFile = DATA_MODULE_TEST_DIR "/data/plink_map/spaces.map";
  CHECK_THROWS_WITH(PlinkMap(spaceSepFile), ContainsSubstring("should contain either 3 or 4 tab-separated columns"));

  // Map file contains too many columns
  std::string fiveCols = DATA_MODULE_TEST_DIR "/data/plink_map/five_cols.map";
  CHECK_THROWS_WITH(PlinkMap(fiveCols), ContainsSubstring("columns, but contains 5"));

  // All genetic positions must be unsigned integers
  std::string noUnsigned = DATA_MODULE_TEST_DIR "/data/plink_map/no_unsigned.map";
  CHECK_THROWS_WITH(PlinkMap(noUnsigned), ContainsSubstring("line 2 column 4: expected unsigned integer but got"));

  // All genetic positions and physical positions must be strictly increasing
  std::string physicalPositions = DATA_MODULE_TEST_DIR "/data/plink_map/physical_positions.map";
  std::string geneticPositions = DATA_MODULE_TEST_DIR "/data/plink_map/genetic_positions.map";
  {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    auto map = PlinkMap(physicalPositions);
    std::string text = buffer.str();
    CHECK_THAT(buffer.str(), ContainsSubstring("physical positions are not strictly increasing"));
    std::cout.rdbuf(old);
  }
  {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    auto map = PlinkMap(geneticPositions);
    std::string text = buffer.str();
    CHECK_THAT(buffer.str(), ContainsSubstring("genetic positions are not increasing"));
    std::cout.rdbuf(old);
  }
}

TEST_CASE("PlinkMap: test good maps", "[PlinkMap]") {

  SECTION("3 column map") {
    std::string mapFile = DATA_MODULE_TEST_DIR "/data/plink_map/3_col.map";
    PlinkMap map{mapFile};

    CHECK(map.getNumSites() == 5ul);
    CHECK(map.getNumCols() == 3ul);
    CHECK(map.getChrIds() == std::vector<std::string>{"abc", "bcd", "cde", "def", "efg"});
    CHECK(map.getSnpIds() == std::vector<std::string>{"SNP_1", "SNP_2", "SNP_3", "SNP_4", "SNP_5"});
    CHECK(map.getGeneticPositions().empty());
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{123ul, 234ul, 345ul, 456ul, 567ul});
  }

  SECTION("4 column map") {
    std::string mapFile = DATA_MODULE_TEST_DIR "/data/plink_map/4_col.map";
    PlinkMap map{mapFile};

    CHECK(map.getNumSites() == 3ul);
    CHECK(map.getNumCols() == 4ul);
    CHECK(map.getChrIds() == std::vector<std::string>{"1", "1", "1"});
    CHECK(map.getSnpIds() ==
          std::vector<std::string>{"SNP_29993579_61334", "SNP_29993696_97083", "SNP_29993781_61335"});
    CHECK(map.getGeneticPositions() == std::vector<double>{0.49943891, 49.94398, 49.944002});
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{29993579ul, 29993696ul, 29993781ul});
  }
}

TEST_CASE("PlinkMap: disambiguate from genetic map", "[PlinkMap]") {

  std::string geneticMapWithoutHeader4 = DATA_MODULE_TEST_DIR "/data/genetic_map/4_col.map";
  std::string geneticMapWithoutHeader3 = DATA_MODULE_TEST_DIR "/data/genetic_map/3_col.map";
  std::string geneticMapWithHeader4 = DATA_MODULE_TEST_DIR "/data/genetic_map/4_col_header.map";

  CHECK_THROWS_WITH(PlinkMap(geneticMapWithoutHeader4), ContainsSubstring("column 4: expected unsigned integer"));
  CHECK_THROWS_WITH(PlinkMap(geneticMapWithoutHeader3), ContainsSubstring("column 3: expected unsigned integer"));
  CHECK_THROWS_WITH(PlinkMap(geneticMapWithHeader4), ContainsSubstring("expected floating point but got"));
}

TEST_CASE("PlinkMap: warn on Mbp-cM range", "[PlinkMap]") {

  std::string mapFile = DATA_MODULE_TEST_DIR "/data/plink_map/4_col.map";

  std::stringstream buffer;
  std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
  auto map = PlinkMap(mapFile);
  CHECK_THAT(buffer.str(), ContainsSubstring("Warning: 33.3% of entries"));
  std::cout.rdbuf(old);
}

} // namespace asmc
