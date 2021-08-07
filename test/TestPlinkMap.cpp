// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "PlinkMap.hpp"

#include <catch2/catch.hpp>

#include <cstdint>
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
}

TEST_CASE("PlinkMap: test good maps", "[PlinkMap]") {

  SECTION("3 column map"){
    std::string mapFile = DATA_MODULE_TEST_DIR "/data/plink_map/3_col.map";
    PlinkMap map{mapFile};

    CHECK(map.getNumSites() == 5ul);
    CHECK(map.getNumCols() == 3ul);
    CHECK(map.getChrIds() == std::vector<std::string>{"abc", "bcd", "cde", "def", "efg"});
    CHECK(map.getSnpIds() == std::vector<std::string>{"SNP_1", "SNP_2", "SNP_3", "SNP_4", "SNP_5"});
    CHECK(map.getGeneticPositions().empty());
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{123ul, 234ul, 345ul, 456ul, 567ul});
  }

  SECTION("4 column map"){
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

} // namespace asmc