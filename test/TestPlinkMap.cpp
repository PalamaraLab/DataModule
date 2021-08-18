// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "PlinkMap.hpp"

#include <catch2/catch.hpp>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <fmt/core.h>

namespace asmc {

TEST_CASE("PlinkMap: test exceptions", "[PlinkMap]") {

  // Map file does not exist
  std::string nonexistentFile = DATA_MODULE_TEST_DIR "/does/not/exist.map";
  CHECK_THROWS_WITH(PlinkMap(nonexistentFile), Catch::StartsWith("Error: PLINK map file"));
  CHECK_THROWS_WITH(PlinkMap(nonexistentFile), Catch::EndsWith("does not exist\n"));

  // Map file is not tab-separated
  std::string spaceSepFile = DATA_MODULE_TEST_DIR "/data/plink_map/spaces.map";
  CHECK_THROWS_WITH(PlinkMap(spaceSepFile), Catch::Contains("should contain either 3 or 4 tab-separated columns"));

  // Map file contains too many columns
  std::string fiveCols = DATA_MODULE_TEST_DIR "/data/plink_map/five_cols.map";
  CHECK_THROWS_WITH(PlinkMap(fiveCols), Catch::Contains("columns, but contains 5"));

  // All genetic positions must be unsigned integers
  std::string noUnsigned = DATA_MODULE_TEST_DIR "/data/plink_map/no_unsigned.map";
  CHECK_THROWS_WITH(PlinkMap(noUnsigned), Catch::Contains("line 2 column 4: expected unsigned integer but got"));

  // All genetic positions and physical positions must be strictly increasing
  std::string physicalPositions = DATA_MODULE_TEST_DIR "/data/plink_map/physical_positions.map";
  std::string geneticPositions = DATA_MODULE_TEST_DIR "/data/plink_map/genetic_positions.map";
  {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    auto map = PlinkMap(physicalPositions);
    std::string text = buffer.str();
    CHECK_THAT(buffer.str(), Catch::Contains("physical positions are not strictly increasing"));
    std::cout.rdbuf(old);
  }
  {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    auto map = PlinkMap(geneticPositions);
    std::string text = buffer.str();
    CHECK_THAT(buffer.str(), Catch::Contains("genetic positions are not increasing"));
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
    CHECK(map.getSnpIds() == std::vector<std::string>{"SNP_1", "SNP_2", "SNP_3"});
    CHECK(map.getGeneticPositions() == std::vector<double>{0.1, 0.2, 0.3});
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{1234ul, 2345ul, 3456ul});
  }
}

TEST_CASE("PlinkMap: disambiguate from genetic map", "[PlinkMap]") {

  std::string geneticMapWithoutHeader4 = DATA_MODULE_TEST_DIR "/data/genetic_map/4_col.map";
  std::string geneticMapWithoutHeader3 = DATA_MODULE_TEST_DIR "/data/genetic_map/3_col.map";
  std::string geneticMapWithHeader4 = DATA_MODULE_TEST_DIR "/data/genetic_map/4_col_header.map";

  CHECK_THROWS_WITH(PlinkMap(geneticMapWithoutHeader4), Catch::Contains("column 4: expected unsigned integer"));
  CHECK_THROWS_WITH(PlinkMap(geneticMapWithoutHeader3), Catch::Contains("column 3: expected unsigned integer"));
  CHECK_THROWS_WITH(PlinkMap(geneticMapWithHeader4), Catch::Contains("expected floating point but got"));
}

} // namespace asmc
