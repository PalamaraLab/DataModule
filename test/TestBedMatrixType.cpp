// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "BedMatrixType.hpp"

#include <catch2/catch.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
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

//  fmt::print("Num sites: {}\n", hapsMatrix.getNumSites());
  CHECK(bedMatrix.getNumSites() == 1980837ul);
  CHECK(bedMatrix.getNumIndividuals() == 100ul);

  auto names = bedMatrix.getSiteNames();
  auto freq = bedMatrix.calculateFrequencies();
  rvec_ul_t NCHROBS = 2ul * (bedMatrix.getNumIndividuals() - bedMatrix.countMissing().array());

  fmt::print("first few: {} {} {}\n", NCHROBS[0], NCHROBS[1], NCHROBS[2]);

  FILE* fp = std::fopen("/home/fergus/GitRepos/PalamaraLab/plink/cpp.freq", "w");

  fmt::print(fp, " CHR           SNP   A1   A2          MAF  NCHROBS\n", freq[0], NCHROBS[0]);

  for (unsigned long i = 0ul; i < bedMatrix.getNumSites(); ++i) {
    auto ii = static_cast<index_t>(i);
    fmt::print(fp, "{:>4}{:>14}{:>5}{:>5}{:>13}{:>9}\n", 1, names.at(i), 1, 2, freq[ii], NCHROBS[ii]);
  }

  std::fclose(fp);

}

} // namespace asmc
