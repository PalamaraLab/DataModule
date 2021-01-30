// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "HapsMatrixType.hpp"
#include "utils/FileUtils.hpp"
#include "utils/StringUtils.hpp"

#include <cassert>
#include <exception>
#include <filesystem>
#include <string_view>
#include <vector>

#include <fmt/core.h>

namespace asmc {

HapsMatrixType HapsMatrixType::createFromHapsPlusSamples(std::string_view hapsFile, std::string_view samplesFile,
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

  instance.readSamplesFile(samplesFile);
  instance.readMapFile(mapFile);
  instance.readHapsFile(hapsFile);

  return instance;
}

void HapsMatrixType::readSamplesFile(const fs::path& samplesFile) {

  auto gzFile = gzopen(samplesFile.string().c_str(), "r");

  // Process first two lines that contain header information
  std::vector<std::string> line1 = splitTextByDelimiter(readNextLineFromGzip(gzFile), " ");
  if (line1.size() < 3 || line1.at(0) != "ID_1" || line1.at(1) != "ID_2" || line1.at(2) != "missing") {
    gzclose(gzFile);
    throw std::runtime_error(
        fmt::format("Expected fist row of .samples file {} to start \"ID_1 ID_2 missing\"", samplesFile.string()));
  }

  std::vector<std::string> line2 = splitTextByDelimiter(readNextLineFromGzip(gzFile), " ");
  if (line2.size() < 3 || line2.at(0) != "0" || line2.at(1) != "0" || line2.at(2) != "0") {
    gzclose(gzFile);
    throw std::runtime_error(
        fmt::format("Expected second row of .samples file {} to start \"0 0 0\"", samplesFile.string()));
  }

  // Read the individuals information
  unsigned long numIndividuals = 0;
  while (!gzeof(gzFile)) {
    std::vector<std::string> line = splitTextByDelimiter(readNextLineFromGzip(gzFile), " ");
    if (!line.empty()) {
      numIndividuals++;
    }
  }

  mNumIndividuals = numIndividuals;

  gzclose(gzFile);
}

void HapsMatrixType::readHapsFile(const fs::path& samplesFile) {
  fmt::print("Reading {}\n", samplesFile.string());
  mNumIndividuals += 0ul;
}

void HapsMatrixType::readMapFile(const fs::path& mapFile) {

  auto gzFile = gzopen(mapFile.string().c_str(), "r");

  while (!gzeof(gzFile)) {
    std::vector<std::string> line = splitTextByDelimiter(readNextLineFromGzip(gzFile), "\t");
    if (!line.empty()) {
      assert(line.size() == 4ul);
      mGeneticPositions.emplace_back(std::stod(line.at(2)));
      mPhysicalPositions.emplace_back(std::stoul(line.at(3)));
    }
  }

  gzclose(gzFile);
}

unsigned long HapsMatrixType::getNumIndividuals() const {
  return mNumIndividuals;
}

unsigned long HapsMatrixType::getNumSites() const {
  return static_cast<unsigned long>(mGeneticPositions.size());
}

const std::vector<unsigned long>& HapsMatrixType::getPhysicalPositions() const {
  return mPhysicalPositions;
}

const std::vector<double>& HapsMatrixType::getGeneticPositions() const {
  return mGeneticPositions;
}

} // namespace asmc
