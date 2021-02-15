// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_HAPS_MATRIX_TYPE_HPP
#define DATA_MODULE_HAPS_MATRIX_TYPE_HPP

#include "utils/EigenTypes.hpp"

#include <filesystem>
#include <string_view>
#include <vector>

#include <fmt/format.h>

namespace asmc {

namespace fs = std::filesystem;

/**
 * A class that stores information in a #sites x #haps matrix of booleans.
 */
class BedMatrixType {

private:
  /** The number of individuals */
  unsigned long mNumIndividuals = 0ul;

  /** The physical positions of each site */
  std::vector<unsigned long> mPhysicalPositions;

  /** The genetic positions, in centimorgans, of each site */
  std::vector<double> mGeneticPositions;

  /** The #sites x #haps matrix of booleans, where #haps is 2x #individuals */
  mat_uint8_t mData;

  /**
   * Read data from the .bed file.
   * @param bedFile path to the .bed file
   */
  void readBedFile(const fs::path& bedFile);

  /**
   * Default constructor.
   */
  BedMatrixType() = default;

public:

  /**
   * Create a BedMatrixType from a .bed, a .bim file, and a .fam file.
   *
   * @param hapsFile path to the .bed file
   * @param samplesFile path to the .bim file
   * @param mapFile path to the .fam file
   * @return instance of a HapsMatrixType
   */
  static BedMatrixType createFromBedBimFam(std::string_view bedFile, std::string_view bimFile,
                                            std::string_view famFile);

  /**
   * @return the number of individuals, determined from the .sample[s] file
   */
  [[nodiscard]] unsigned long getNumIndividuals() const;

  /**
   * @return the number of sites, determined from the .map file
   */
  [[nodiscard]] unsigned long getNumSites() const;

  /**
   * @return a vector of physical positions, read in from the .map file
   */
  [[nodiscard]] const std::vector<unsigned long>& getPhysicalPositions() const;

  /**
   * @return a vector of genetic positions, in centimorgans, read in from the .map file
   */
  [[nodiscard]] const std::vector<double>& getGeneticPositions() const;

  /**
   * @return the vector of raw boolean data, contained in the .hap[s][.gz] file
   */
  [[nodiscard]] const mat_uint8_t& getData() const;

  /**
   * Get all haplotype data for a single site. This is a row from the data matrix and will be a boolean row vector of
   * length 2N where N is the number of individuals.
   * @param siteId the id of the site
   * @return the ith row of the data matrix, where i is siteId.
   */
  [[nodiscard]] rvec_uint8_t getSite(unsigned long siteId) const;

  /**
   * Get all site data for a single haplotype. This is a column from the data matrix and will be a boolean column vector
   * of length N where N is the number of sites.
   * @param hapId the id of the haplotype
   * @return the jth row of the data matrix, where j is hapId.
   */
  [[nodiscard]] cvec_uint8_t getHap(unsigned long hapId) const;
};

} // namespace asmc

#endif // DATA_MODULE_HAPS_MATRIX_TYPE_HPP
