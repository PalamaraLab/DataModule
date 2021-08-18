// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "GeneticMap.hpp"

#include <catch2/catch.hpp>

#include <cstdint>
#include <string>

#include <fmt/core.h>

namespace asmc {

TEST_CASE("GeneticMap: test exceptions", "[GeneticMap]") {

  // Map file does not exist
  std::string nonexistentFile = DATA_MODULE_TEST_DIR "/does/not/exist.map";
  CHECK_THROWS_WITH(GeneticMap(nonexistentFile), Catch::StartsWith("Error: genetic map file"));
  CHECK_THROWS_WITH(GeneticMap(nonexistentFile), Catch::EndsWith("does not exist\n"));

  // Map file is not tab-separated
  std::string spaceSepFile = DATA_MODULE_TEST_DIR "/data/genetic_map/spaces.map";
  CHECK_THROWS_WITH(GeneticMap(spaceSepFile),
                    Catch::Contains("should contain at least one data row with at least 3 tab-separated columns"));

  // Map file contains lines with differing number of columns
  std::string inconsistentNumCols = DATA_MODULE_TEST_DIR "/data/genetic_map/inconsistent_num_cols.map";
  CHECK_THROWS_WITH(GeneticMap(inconsistentNumCols),
                    Catch::Contains("line 2 contains 3 columns, but the first data row contains 4"));

  // Column 1 should contain unsigned integers, column 3 should contain floats
  std::string badCol1 = DATA_MODULE_TEST_DIR "/data/genetic_map/bad_physical_pos.map";
  std::string badCol3 = DATA_MODULE_TEST_DIR "/data/genetic_map/bad_genetic_pos.map";
  CHECK_THROWS_WITH(GeneticMap(badCol1), Catch::Contains("line 3 should contain an unsigned integer"));
  CHECK_THROWS_WITH(GeneticMap(badCol3), Catch::Contains("line 4 should contain an unsigned integer"));

  // All genetic positions and physical positions must be strictly increasing
  std::string physicalPositions = DATA_MODULE_TEST_DIR "/data/genetic_map/physical_positions.map";
  std::string geneticPositions = DATA_MODULE_TEST_DIR "/data/genetic_map/genetic_positions.map";
  CHECK_THROWS_WITH(GeneticMap(physicalPositions), Catch::Contains("physical positions are not strictly increasing"));
  CHECK_THROWS_WITH(GeneticMap(geneticPositions), Catch::Contains("genetic positions are not increasing"));
}

TEST_CASE("GeneticMap: test good maps", "[GeneticMap]") {

  SECTION("3 column map without header") {
    std::string mapFile = DATA_MODULE_TEST_DIR "/data/genetic_map/3_col.map";
    GeneticMap map{mapFile};

    CHECK(map.getNumSites() == 5ul);
    CHECK(map.getNumCols() == 3ul);
    CHECK(!map.hasHeader());
    CHECK(map.getGeneticPositions() == std::vector<double>{0.0, 0.117, 0.125, 0.126, 0.140});
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{1ul, 24ul, 26ul, 27ul, 31ul});
  }

  SECTION("3 column map with header") {
    std::string mapFile = DATA_MODULE_TEST_DIR "/data/genetic_map/3_col_header.map";
    GeneticMap map{mapFile};

    CHECK(map.getNumSites() == 5ul);
    CHECK(map.getNumCols() == 3ul);
    CHECK(map.hasHeader());
    CHECK(map.getGeneticPositions() == std::vector<double>{0.0, 0.117, 0.125, 0.126, 0.140});
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{1ul, 24ul, 26ul, 27ul, 31ul});
  }

  SECTION("4 column map without header") {
    std::string mapFile = DATA_MODULE_TEST_DIR "/data/genetic_map/4_col.map";
    GeneticMap map{mapFile};

    CHECK(map.getNumSites() == 5ul);
    CHECK(map.getNumCols() == 4ul);
    CHECK(!map.hasHeader());
    CHECK(map.getGeneticPositions() == std::vector<double>{0.22, 0.30, 0.31, 0.32, 0.45});
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{58ul, 82ul, 85ul, 88ul, 110ul});
  }

  SECTION("4 column map with header") {
    std::string mapFile = DATA_MODULE_TEST_DIR "/data/genetic_map/4_col_header.map";
    GeneticMap map{mapFile};

    CHECK(map.getNumSites() == 5ul);
    CHECK(map.getNumCols() == 4ul);
    CHECK(map.hasHeader());
    CHECK(map.getGeneticPositions() == std::vector<double>{0.22, 0.30, 0.31, 0.32, 0.45});
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{58ul, 82ul, 85ul, 88ul, 110ul});
  }
}

TEST_CASE("GeneticMap: disambiguate from PLINK map", "[GeneticMap]") {

  std::string plinkMap3Col = DATA_MODULE_TEST_DIR "/data/plink_map/4_col.map";
  std::string plinkMap4Col = DATA_MODULE_TEST_DIR "/data/plink_map/3_col.map";

  CHECK_THROWS_WITH(GeneticMap(plinkMap3Col), Catch::Contains("physical positions are not strictly increasing"));
  CHECK_THROWS_WITH(GeneticMap(plinkMap4Col), Catch::Contains("should contain at least one data row with at least 3"));
}

} // namespace asmc
