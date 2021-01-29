// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "HapsMatrixType.hpp"
#include "FileFormats.hpp"
#include "Utils.hpp"

#include <exception>
#include <filesystem>
#include <string_view>
#include <vector>

#include <fmt/core.h>

#include <iostream>

namespace asmc {


void HapsMatrixType::CreateFromHapsPlusSamples(std::string_view filePath) {

  auto fileDir = fs::path{filePath}.parent_path();

  if (!fs::exists(fileDir)) {
    throw std::runtime_error(
        fmt::format("Expected path to haps/samples files, but {} does not exist", fileDir.string()));
  }

  auto samplesFilePath = getFileInDirWithExt(filePath, {".samples", ".sample"});
  auto hapsFilePath = getFileInDirWithExt(filePath, {".hap.gz", ".hap", ".haps.gz", ".haps"});

  if(!fs::exists(samplesFilePath)) {
    throw std::runtime_error(fmt::format("Cannot find .sample[s] file in {}", fileDir.string()));
  }

  if(!fs::exists(hapsFilePath)) {
    throw std::runtime_error(fmt::format("Cannot find .hap[s][.gz] file in {}", fileDir.string()));
  }

  ReadSamplesFile(samplesFilePath);
  ReadHapsFile(samplesFilePath);
}

void HapsMatrixType::ReadSamplesFile(const fs::path& file) {
  fmt::print("Reading {}\n", file.string());
}

void HapsMatrixType::ReadHapsFile(const fs::path& file) {
  fmt::print("Reading {}\n", file.string());
}

HapsMatrixType::HapsMatrixType(FileFormat fileFormat, std::string_view filePath) {

  switch (fileFormat) {
  case FileFormat::HapsPlusSamples:
    CreateFromHapsPlusSamples(filePath);
    break;
  }
}

} // namespace asmc
