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
  void ReadSamplesFile(const fs::path& samplesFile);

  void ReadHapsFile(const fs::path& samplesFile);

  HapsMatrixType() = default;

public:
  static HapsMatrixType CreateFromHapsPlusSamples(std::string_view hapsFile, std::string_view samplesFile,
                                                  std::string_view mapFile);

  void print() const {
    fmt::print("testing...\n");
  }
};

} // namespace asmc

#endif // DATA_MODULE_HAPS_MATRIX_TYPE_HPP
