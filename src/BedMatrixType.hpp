// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_BED_MATRIX_TYPE_HPP
#define DATA_MODULE_BED_MATRIX_TYPE_HPP

#include "utils/EigenTypes.hpp"

#include <chrono>

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

  std::chrono::milliseconds tReadBed = {};
  std::chrono::milliseconds tReadBim = {};
  std::chrono::milliseconds tReadFam = {};
  std::chrono::milliseconds tWriteFrq = {};

  /** The number of individuals */
  unsigned long mNumIndividuals = 0ul;

  /** The names of each site */
  std::vector<std::string> mSiteNames;

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
   * Read data from the .bim file.
   * @param bimFile path to the .bim file
   */
  void readBimFile(const fs::path& bimFile);

  /**
   * Read data from the .fam file.
   * @param famFile path to the .fam file
   */
  void readFamFile(const fs::path& famFile);

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
   * @return a vector of physical positions, read in from the .bim file
   */
  [[nodiscard]] const std::vector<unsigned long>& getPhysicalPositions() const;

  /**
   * @return a vector of genetic positions, in centimorgans, read in from the .bim file
   */
  [[nodiscard]] const std::vector<double>& getGeneticPositions() const;

  /**
   * @return a vector of variant names, read in from the .bim file
   */
  [[nodiscard]] const std::vector<std::string>& getSiteNames() const;

  /**
   * @return the vector of raw boolean data, contained in the .hap[s][.gz] file
   */
  [[nodiscard]] const mat_uint8_t& getData() const;

  /**
   * Get all variant data for a single individual.
   *
   * @param individualId the id of the individual
   * @return the ith row of the data matrix, where i is individualId.
   */
  [[nodiscard]] rvec_uint8_t getIndividual(unsigned long individualId) const;

  /**
   * Calculate the site-wise number of missing entries.
   *
   * @return a row vector containing count of missing data across all individuals for each site.
   */
  [[nodiscard]] rvec_ul_t countMissing() const;

  /**
   * Calculate frequencies.
   *
   * @return a row vector containing frequencies across all individuals for each site.
   */
  [[nodiscard]] rvec_dbl_t calculateFrequencies() const;

  /**
   * Write a .frq file containing the frequency data.
   *
   * @param frqFile path to the .frq file to write to
   */
  void writeFrequencies(std::string_view frqFile);

  /**
   * Get all individual data for a single site.
   *
   * @param siteId the id of the site
   * @return the jth column of the data matrix, where j is siteId.
   */
  [[nodiscard]] cvec_uint8_t getSite(unsigned long siteId) const;

  [[nodiscard]] std::string printTiming() const {
    fmt::memory_buffer out;
    fmt::format_to(out, "###\nReading bed:  {:>6} ms\n", tReadBed.count());
    fmt::format_to(out, "Reading bim:  {:>6} ms\n", tReadBim.count());
    fmt::format_to(out, "Reading fam:  {:>6} ms\n", tReadFam.count());
    fmt::format_to(out, "Writing frq:  {:>6} ms\n###\n", tWriteFrq.count());
    return fmt::to_string(out);
  }
};

} // namespace asmc

#endif // DATA_MODULE_BED_MATRIX_TYPE_HPP