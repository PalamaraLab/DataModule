// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "HapsMatrixType.hpp"
#include "Utils.hpp"

#include <exception>
#include <filesystem>
#include <string_view>
#include <vector>

#include <fmt/core.h>

#include <iostream>

namespace asmc {

HapsMatrixType HapsMatrixType::CreateFromHapsPlusSamples(std::string_view hapsFile, std::string_view samplesFile,
                                                         std::string_view mapFile) {

  if (!fs::exists(hapsFile) || !fs::is_regular_file(hapsFile)) {
    throw std::runtime_error(fmt::format("Expected .hap[s][.gz] file, but got {}", hapsFile));
  }
  if (!fs::exists(samplesFile) || !fs::is_regular_file(samplesFile)) {
    throw std::runtime_error(fmt::format("Expected .sample[s] file, but got {}", samplesFile));
  }
  if (!fs::exists(mapFile) || !fs::is_regular_file(mapFile)) {
    throw std::runtime_error(fmt::format("Expected .map file, but got {}", mapFile));
  }

  HapsMatrixType instance;

  instance.ReadSamplesFile(samplesFile);
  instance.ReadHapsFile(hapsFile);

  return instance;
}

void HapsMatrixType::ReadSamplesFile(const fs::path& file) {

  unsigned long numRecords = 0;

  auto gzFile = gzopen(file.c_str(), "r");

  // Process first two lines that contain header information
  {
    // First two lines must have at least 3 entries
    std::vector<std::string> line1 = splitTextByDelimiter(readNextLineFromGzip(gzFile), " ");
    std::vector<std::string> line2 = splitTextByDelimiter(readNextLineFromGzip(gzFile), " ");
    if (line1.size() < 3 || line2.size() < 3) {
      throw std::runtime_error(fmt::format(
          "Expected {} to start with two header rows containing at least space-delimited columns", file.string()));
    }

    if (line2.at(0) != "0" || line2.at(1) != "0" || line2.at(2) != "0") {
      throw std::runtime_error(fmt::format("Expected the second row of {} to start \"0 0 0\"", file.string()));
    }
  }

  while (!gzeof(gzFile)) {
    std::vector<std::string> line = splitTextByDelimiter(readNextLineFromGzip(gzFile), " ");
    if (!line.empty()) {
      numRecords++;
    }
  }

  gzclose(gzFile);
}

void HapsMatrixType::ReadHapsFile(const fs::path& file) {
  fmt::print("Reading {}\n", file.string());
}

} // namespace asmc
