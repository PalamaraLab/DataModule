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
class HapsMatrixType {

private:

  /** The number of individuals */
  unsigned long mNumIndividuals = 0ul;

  /** The physical positions of each site */
  std::vector<unsigned long> mPhysicalPositions;

  /** The genetic positions, in centimorgans, of each site */
  std::vector<double> mGeneticPositions;

  /** The #sites x #haps matrix of booleans, where #haps is 2x #individuals */
  mat_bool_t mData;

  /**
   * Read data out of the .sample[s] file, which contains metadata about each individual.
   * @param samplesFile path to the .sample[s] file
   */
  void readSamplesFile(const fs::path& samplesFile);

  /**
   * Read data out of the .hap[s][.gz] file, which contains #sites rows, and 5 + 2 * #individuals columns. The first 5
   * columns contain metadata, followed by two columns of boolean values per individual.
   * @param hapsFile path to the .hap[s][.gz] file
   */
  void readHapsFile(const fs::path& hapsFile);

  /**
   * Read data from the .map file, which contains genetic and physical positions for each site.
   * @param mapFile path to the .map file
   */
  void readMapFile(const fs::path& mapFile);

  /**
   * Validate the haps file by checking:
   *  1. every row contains 5 + 2N columns, where N is the number of individuals
   *  2. every row contains only boolean values in the haps columns
   *  3. the number of rows is equal to the number of sites, determined from the .map file
   * @param hapsFile path to the .hap[s][.gz] file
   */
  void validateHapsFile(const fs::path& hapsFile);

  /**
   * Validate an individual row from the .hap[s][.gz] file:
   *  1. it contains exactly 5 + 2N columns, where N is the number of individuals
   *  2. it contains only boolean values in the haps columns
   * @param row the row read from the .hap[s][.gz] file
   */
  void validateHapsRow(const std::vector<std::string>& row) const;

  /**
   * Default constructor.
   */
  HapsMatrixType() = default;

public:

  /**
   * Create a HapsMatrixType from a .hap[s][.gz], a .sample[s] file, and a .map file.
   *
   * @param hapsFile path to the .hap[s][.gz] file
   * @param samplesFile path to the .sample[s] file
   * @param mapFile path to the .map file
   * @return instance of a HapsMatrixType
   */
  static HapsMatrixType createFromHapsPlusSamples(std::string_view hapsFile, std::string_view samplesFile,
                                                  std::string_view mapFile);

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
  [[nodiscard]] const mat_bool_t& getData() const;

  /**
   * Get all haplotype data for a single site. This is a row from the data matrix and will be a boolean row vector of
   * length 2N where N is the number of individuals.
   * @param siteId the id of the site
   * @return the ith row of the data matrix, where i is siteId.
   */
  [[nodiscard]] rvec_bool_t getSite(unsigned long siteId) const;

  /**
   * Get all site data for a single haplotype. This is a column from the data matrix and will be a boolean column vector
   * of length N where N is the number of sites.
   * @param hapId the id of the haplotype
   * @return the jth row of the data matrix, where j is hapId.
   */
  [[nodiscard]] cvec_bool_t getHap(unsigned long hapId) const;
};

} // namespace asmc

#endif // DATA_MODULE_HAPS_MATRIX_TYPE_HPP
