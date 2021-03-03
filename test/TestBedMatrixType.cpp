// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "BedMatrixType.hpp"

#include <catch2/catch.hpp>

#include <cstdint>
#include <string>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>

namespace asmc {

TEST_CASE("BedMatrixType: test bed read", "[BedMatrixType]") {

  std::string bedFile = "/home/fergus/GitRepos/PalamaraLab/plink/data/bedbimfam/chr.merge.bed";
  std::string bimFile = "/home/fergus/GitRepos/PalamaraLab/plink/data/bedbimfam/chr.merge.bim";
  std::string famFile = "/home/fergus/GitRepos/PalamaraLab/plink/data/bedbimfam/chr.merge.fam";

  auto bedMatrix = BedMatrixType::createFromBedBimFam(bedFile, bimFile, famFile);

  CHECK(bedMatrix.getNumSites() == 1980837ul);
  CHECK(bedMatrix.getNumIndividuals() == 100ul);

  bedMatrix.writeFrequencies("/home/fergus/GitRepos/PalamaraLab/plink/data/bedbimfam/chr.merge.frq");
  fmt::print(bedMatrix.printTiming());
}

} // namespace asmc
