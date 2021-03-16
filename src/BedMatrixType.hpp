// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_BED_MATRIX_TYPE_HPP
#define DATA_MODULE_BED_MATRIX_TYPE_HPP

#include "utils/EigenTypes.hpp"

#include <chrono>

#include <filesystem>
#include <limits>
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

  /** The detected delimeter used in the .fam file */
  std::string mFamDelimeter = " ";

  /** The value of missing data in integer format */
  const long mMissingInt = 3l;

  /** The value of missing data in floating format */
  const float mMissingFloat = std::numeric_limits<float>::quiet_NaN();

  /** A row vector of the number of missing pieces of data for each site */
  rvec_ul_t mMissingCounts;

  /** Determine the appropriate delimiters for the .fam file */
  void determineFamDelimiter(const fs::path& famFile);

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
   * Get the raw allele count for a given site.
   * @param siteId the site ID
   * @return the number of 1s in row #sideId
   */
  [[nodiscard]] unsigned long getAlleleCount(unsigned long siteId) const;

  /**
   * Get the raw allele counts for all sites.
   * @return a vector of counts, one for each site
   */
  [[nodiscard]] rvec_ul_t getAlleleCounts() const;

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
   * @return the number of individuals, determined from the .fam file
   */
  [[nodiscard]] unsigned long getNumIndividuals() const;

  /**
   * @return the number of sites, determined from the .bim file
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
   * @return the vector of raw uint8_t data, contained in the .bed file with 3 representing missing data
   */
  [[nodiscard]] const mat_uint8_t& getData() const;

  /**
   * @return the vector of raw float data, with NAN representing missing data
   */
  [[nodiscard]] mat_float_t getDataAsFloat() const;

  /**
   * Get all variant data for a single individual.
   *
   * @param individualId the id of the individual
   * @return the ith row of the data matrix, where i is individualId.
   */
  [[nodiscard]] rvec_uint8_t getIndividual(unsigned long individualId) const;

  /**
   * Get all individual data for a single site.
   *
   * @param siteId the id of the site
   * @return the jth column of the data matrix, where j is siteId.
   */
  [[nodiscard]] cvec_uint8_t getSite(unsigned long siteId) const;

  /**
   * Get the count of missing data for a given site.
   * @param siteId the site ID
   * @return count of missing data at the given site
   */
  [[nodiscard]] unsigned long getMissingCount(unsigned long siteId) const;

  /**
   * Get the counts of missing data for all sites.
   * @return counts of missing data for all sites
   */
  [[nodiscard]] rvec_ul_t getMissingCounts() const;

  /**
   * Get the frequency of missing data for a given site.
   * @param siteId the site ID
   * @return frequency of missing data at the given site
   */
  [[nodiscard]] double getMissingFrequency(unsigned long siteId) const;

  /**
   * Get the frequencies of missing data for all sites.
   * @return frequencies of missing data at all sites
   */
  [[nodiscard]] rvec_dbl_t getMissingFrequencies() const;

  /**
   * Get the minor allele count for a given site. This is a number in [0, #haps/2].
   * @param siteId the site ID
   * @return the minor allele count for the given site
   */
  [[nodiscard]] unsigned long getMinorAlleleCount(unsigned long siteId) const;

  /**
   * Get the raw minor allele counts for all sites. Each is a number in [0, #haps/2].
   * @return a vector of minor allele counts, one for each site
   */
  [[nodiscard]] rvec_ul_t getMinorAlleleCounts() const;

  /**
   * Get the derived allele count for a given site. This is the raw count, assuming 1 means derived.
   * @param siteId the site ID
   * @return the derived allele count for the given site
   */
  [[nodiscard]] unsigned long getDerivedAlleleCount(unsigned long siteId) const;

  /**
   * Get the derived allele count for all sites. These are the raw counts, assuming 1 means derived.
   * @return a vector of derived allele counts, one for each site
   */
  [[nodiscard]] rvec_ul_t getDerivedAlleleCounts() const;

  /**
   * Get the minor allele frequency for a given site. This is a number in [0, 0.5].
   * @param siteId the site ID
   * @return the minor allele frequency for the given site
   */
  [[nodiscard]] double getMinorAlleleFrequency(unsigned long siteId) const;

  /**
   * Get the derived allele frequency for a given site. This is the raw frequency, assuming 1 means derived.
   * @param siteId the site ID
   * @return the derived allele frequency for the given site
   */
  [[nodiscard]] double getDerivedAlleleFrequency(unsigned long siteId) const;

  /**
   * Get the minor allele frequencies for all sites. These are numbers in [0, 0.5].
   * @return a vector of minor allele frequencies, one for each site
   */
  [[nodiscard]] cvec_dbl_t getMinorAlleleFrequencies() const;

  /**
   * Get the derived allele frequencies for all sites. These are the raw frequencies, assuming 1 means derived.
   * @return a vector of minor allele frequencies, one for each site
   */
  [[nodiscard]] cvec_dbl_t getDerivedAlleleFrequencies() const;

  /**
   * Write a .frq file containing the frequency data.
   *
   * @param frqFile path to the .frq file to write to
   */
  void writeFrequencies(std::string_view frqFile);

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
