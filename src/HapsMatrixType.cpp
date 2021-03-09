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
#include <fmt/ranges.h>
#include <iostream>

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

void HapsMatrixType::readHapsFile(const fs::path& hapsFile) {

  // Check that haps file is the expected shape, and size the data matrix appropriately
  validateHapsFile(hapsFile);
  mData.resize(static_cast<index_t>(getNumSites()), static_cast<index_t>(2ul * mNumIndividuals));

  auto gzFile = gzopen(hapsFile.string().c_str(), "r");

  for (index_t rowId = 0l; rowId < static_cast<index_t>(getNumSites()); ++rowId) {
    std::vector<std::string> line = splitTextByDelimiter(readNextLineFromGzip(gzFile), " ");
    assert(line.size() == 2ul * mNumIndividuals + 5ul);
    for (index_t colId = 0l; colId < static_cast<index_t>(2ul * mNumIndividuals); ++colId) {
      assert(line[static_cast<size_t>(5l + colId)].size() == 1ul);
      mData(rowId, colId) = line[static_cast<size_t>(5l + colId)] == "1";
    }
  }

  gzclose(gzFile);
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

void HapsMatrixType::validateHapsFile(const fs::path& hapsFile) {

  auto gzFile = gzopen(hapsFile.string().c_str(), "r");
  unsigned long linesInFile = 0ul;

  // Get as many lines as we expect are valid, and check that they are valid
  for (unsigned long siteId = 0; siteId < getNumSites(); ++siteId) {
    try {
      validateHapsRow(splitTextByDelimiter(readNextLineFromGzip(gzFile), " "));
    } catch (const std::runtime_error& e) {
      gzclose(gzFile);
      throw std::runtime_error(fmt::format("Error on line {} of {}:\n{}", 1ul + siteId, hapsFile.string(), e.what()));
    }
    linesInFile++;
  }

  // Check for any extra lines, other than a possible expected newline at the end of the file
  while (!gzeof(gzFile)) {
    if (!readNextLineFromGzip(gzFile).empty()) {
      linesInFile++;
    }
  }

  // Error if there are the wrong number of lines
  if (linesInFile != getNumSites()) {
    gzclose(gzFile);
    throw std::runtime_error(
        fmt::format("Expected {} to contain {} lines, but found {}", hapsFile.string(), getNumSites(), linesInFile));
  }

  gzclose(gzFile);
}

void HapsMatrixType::validateHapsRow(const std::vector<std::string>& row) const {

  // Check that the row contains the correct number of elements
  const unsigned long expectedNumCols = 2ul * mNumIndividuals + 5ul;
  if (row.size() != expectedNumCols) {
    throw std::runtime_error(fmt::format("Expected row to contain to contain 2x{}+5={} entries, but found {}",
                                         mNumIndividuals, expectedNumCols, row.size()));
  }

  // Check that the 2N boolean values are either 0 or 1
  for (unsigned long col = 5; col < expectedNumCols; ++col) {
    if (!(row[col] == "0" || row[col] == "1"))
      throw std::runtime_error(
          fmt::format("Expected row to contain to boolean data, but column {} was \"{}\"", 1ul + col, row[col]));
  }
}

unsigned long HapsMatrixType::getNumIndividuals() const {
  return mNumIndividuals;
}

unsigned long HapsMatrixType::getNumHaps() const {
  return 2ul * mNumIndividuals;
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

const mat_uint8_t& HapsMatrixType::getData() const {
  return mData;
}

mat_float_t HapsMatrixType::getDataAsFloat() const {
  return mData.cast<float>();
}

rvec_uint8_t HapsMatrixType::getSite(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return mData.row(static_cast<index_t>(siteId));
}

cvec_uint8_t HapsMatrixType::getHap(unsigned long hapId) const {
  assert(hapId < getNumHaps());
  return mData.col(static_cast<index_t>(hapId));
}

mat_uint8_t HapsMatrixType::getIndividual(unsigned long individualId) const {
  assert(individualId < getNumIndividuals());
  return mData.block(static_cast<index_t>(0), static_cast<index_t>(2ul * individualId), mData.rows(),
                     static_cast<index_t>(2ul));
}

unsigned long HapsMatrixType::getAlleleCount(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return mData.row(static_cast<index_t>(siteId)).cast<unsigned long>().sum();
}

unsigned long HapsMatrixType::getMinorAlleleCount(unsigned long siteId) const {
  assert(siteId < getNumSites());
  const unsigned long rawCount = getAlleleCount(siteId);
  return rawCount <= getNumIndividuals() ? rawCount : getNumHaps() - rawCount;
}

unsigned long HapsMatrixType::getDerivedAlleleCount(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return getAlleleCount(siteId);
}

cvec_ul_t HapsMatrixType::getAlleleCounts() const {
  return mData.cast<unsigned long>().rowwise().sum();
}

cvec_ul_t HapsMatrixType::getMinorAlleleCounts() const {
  cvec_ul_t counts = getAlleleCounts();
  const auto threshold = getNumIndividuals();
  const auto max = getNumHaps();
  return (counts.array() > threshold).select(max - counts.array(), counts);
}

cvec_ul_t HapsMatrixType::getDerivedAlleleCounts() const {
  return getAlleleCounts();
}

double HapsMatrixType::getMinorAlleleFrequency(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return static_cast<double>(getMinorAlleleCount(siteId)) / static_cast<double>(getNumHaps());
}

double HapsMatrixType::getDerivedAlleleFrequency(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return static_cast<double>(getDerivedAlleleCount(siteId)) / static_cast<double>(getNumHaps());
}

cvec_dbl_t HapsMatrixType::getMinorAlleleFrequencies() const {
  return getMinorAlleleCounts().cast<double>().array() / static_cast<double>(getNumHaps());
}

cvec_dbl_t HapsMatrixType::getDerivedAlleleFrequencies() const {
  return getDerivedAlleleCounts().cast<double>().array() / static_cast<double>(getNumHaps());
}

} // namespace asmc
