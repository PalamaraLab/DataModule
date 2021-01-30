// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_HAPS_MATRIX_TYPE_HPP
#define DATA_MODULE_HAPS_MATRIX_TYPE_HPP

#include <filesystem>
#include <string_view>

#include <fmt/format.h>

namespace asmc {

namespace fs = std::filesystem;

class HapsMatrixType {

private:

  unsigned long mNumIndividuals = 0ul;
  std::vector<unsigned long> mPhysicalPositions;
  std::vector<double> mGeneticPositions;

  void readSamplesFile(const fs::path& samplesFile);

  void readHapsFile(const fs::path& samplesFile);

  void readMapFile(const fs::path& mapFile);

  HapsMatrixType() = default;

public:
  static HapsMatrixType createFromHapsPlusSamples(std::string_view hapsFile, std::string_view samplesFile,
                                                  std::string_view mapFile);

  [[nodiscard]] unsigned long getNumIndividuals() const;
  [[nodiscard]] unsigned long getNumSites() const;
  [[nodiscard]] const std::vector<unsigned long>& getPhysicalPositions() const;
  [[nodiscard]] const std::vector<double>& getGeneticPositions() const;
};

} // namespace asmc

#endif // DATA_MODULE_HAPS_MATRIX_TYPE_HPP
