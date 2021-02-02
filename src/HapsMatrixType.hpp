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
   *  1. the first row contains 5 + 2N columns
   *  2. the number of rows is equal to the number of sites, determined from the .map file
   * @param hapsFile path to the .hap[s][.gz] file
   */
  void validateHapsFile(const fs::path& hapsFile);

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
};

} // namespace asmc

#endif // DATA_MODULE_HAPS_MATRIX_TYPE_HPP
