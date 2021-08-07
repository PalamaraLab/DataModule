// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_PLINK_MAP_HPP
#define DATA_MODULE_PLINK_MAP_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>

namespace asmc {

namespace fs = std::filesystem;

/**
 * A class that reads and stores a PLINK map.
 */
class PlinkMap {

private:
  fs::path mInputFile;
  unsigned long mNumSites{};
  unsigned long mNumCols{};
  std::vector<std::string> mChrIds;
  std::vector<std::string> mSnpIds;
  std::vector<double> mGeneticPositions;
  std::vector<unsigned long> mPhysicalPositions;

  /**
   *
   */
  void validateFile();

  /**
   *
   */
  void readFile();

public:
  /**
   * Read a PLINK .map file: a text file with no header file, and one line per variant with the following 3-4 fields:
   * 1. chromosome code / ID (string)
   * 2. variant/SNP identifier (string)
   * 3. (optional) genetic position in either Morgans or Centimorgans; ASMC assumes Centimorgans (float)
   * 4. physical position in base pairs (integer)
   * All lines must have the same number of columns.
   *
   * @param mapFile path to the .map file
   */
  explicit PlinkMap(std::string_view mapFile);

  [[nodiscard]] unsigned long getNumSites() const;
  [[nodiscard]] unsigned long getNumCols() const;
  [[nodiscard]] const std::vector<std::string>& getChrIds() const;
  [[nodiscard]] const std::vector<std::string>& getSnpIds() const;
  [[nodiscard]] const std::vector<double>& getGeneticPositions() const;
  [[nodiscard]] const std::vector<unsigned long>& getPhysicalPositions() const;
};

} // namespace asmc

#endif // DATA_MODULE_PLINK_MAP_HPP
