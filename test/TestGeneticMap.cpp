// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "GeneticMap.hpp"

#include <catch2/catch.hpp>

#include <cstdint>
#include <iostream>
#include <sstream>
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
  {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    auto map = GeneticMap(physicalPositions);
    std::string text = buffer.str();
    CHECK_THAT(buffer.str(), Catch::Contains("physical positions are not strictly increasing"));
    std::cout.rdbuf(old);
  }
  {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    auto map = GeneticMap(geneticPositions);
    std::string text = buffer.str();
    CHECK_THAT(buffer.str(), Catch::Contains("genetic positions are not increasing"));
    std::cout.rdbuf(old);
  }
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
    CHECK(map.getGeneticPositions() == std::vector<double>{0.0286994674, 0.0877781, 0.0878126, 0.0878132, 0.0878152});
    CHECK(map.getPhysicalPositions() == std::vector<unsigned long>{138957ul, 139189ul, 140286ul, 140309ul, 140378ul});
  }
}

TEST_CASE("GeneticMap: disambiguate from PLINK map", "[GeneticMap]") {

  std::string plinkMap3Col = DATA_MODULE_TEST_DIR "/data/plink_map/4_col.map";
  std::string plinkMap4Col = DATA_MODULE_TEST_DIR "/data/plink_map/3_col.map";

  {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    auto map = GeneticMap(plinkMap3Col);
    CHECK_THAT(buffer.str(), Catch::Contains("Warning: genetic map file") && Catch::Contains("indices 0 and 1"));
    std::cout.rdbuf(old);
  }

  CHECK_THROWS_WITH(GeneticMap(plinkMap4Col), Catch::Contains("should contain at least one data row with at least 3"));
}

TEST_CASE("GeneticMap: warn on Mbp-cM range", "[GeneticMap]") {

  std::string mapFile = DATA_MODULE_TEST_DIR "/data/genetic_map/4_col_header.map";

  std::stringstream buffer;
  std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
  auto map = GeneticMap(mapFile);
  CHECK_THAT(buffer.str(), Catch::Contains("Warning: 20.0% of entries"));
  std::cout.rdbuf(old);
}

} // namespace asmc
