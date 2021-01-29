// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_HAPS_MATRIX_TYPE_HPP
#define DATA_MODULE_HAPS_MATRIX_TYPE_HPP

#include "FileFormats.hpp"

#include <filesystem>
#include <string_view>

namespace asmc {

namespace fs = std::filesystem;

class HapsMatrixType {

private:
  void CreateFromHapsPlusSamples(std::string_view filePath);

  void ReadSamplesFile(const fs::path& samplesFile);

  void ReadHapsFile(const fs::path& samplesFile);


public:
  HapsMatrixType(FileFormat fileFormat, std::string_view filePath);

};

} // namespace asmc

#endif // DATA_MODULE_HAPS_MATRIX_TYPE_HPP
