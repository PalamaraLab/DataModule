// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "PlinkMap.hpp"

#include "utils/FileUtils.hpp"
#include "utils/StringUtils.hpp"

#include <exception>

#include <fmt/core.h>

namespace asmc {

PlinkMap::PlinkMap(std::string_view mapFile) : mInputFile{mapFile} {
  validateFile();
  readFile();
}

void PlinkMap::validateFile() {

  // Check file exists
  if (!fs::is_regular_file(mInputFile)) {
    throw std::runtime_error(fmt::format("Error: PLINK map file {} does not exist\n", mInputFile.string()));
  }

  // Check that the file contains either 3 or 4 tab-separated columns
  auto gzFile = gzopen(mInputFile.string().c_str(), "r");
  std::vector<std::string> firstLine = splitTextByDelimiter(readNextLineFromGzip(gzFile), "\t");
  mNumCols = static_cast<unsigned long>(firstLine.size());

  if (!(mNumCols == 3ul || mNumCols == 4ul)) {
    throw std::runtime_error(
        fmt::format("Error: PLINK map file {} should contain either 3 or 4 tab-separated columns, but contains {}\n",
                    mInputFile.string(), mNumCols));
  }

  // Count the number of lines in the file
  mNumSites = 1ul;
  while (!gzeof(gzFile)) {
    std::string line = readNextLineFromGzip(gzFile);
    if (!stripBack(line).empty()) {
      mNumSites++;
    }
  }

  gzclose(gzFile);
}

void PlinkMap::readFile() {
  // Reserve space in vectors
  mChrIds.reserve(mNumSites);
  mSnpIds.reserve(mNumSites);
  if (mNumCols == 4ul) {
    mGeneticPositions.reserve(mNumSites);
  }
  mPhysicalPositions.reserve(mNumSites);

  const unsigned long chrCol = 0ul;
  const unsigned long snpCol = 1ul;
  const unsigned long genCol = 2ul;
  const unsigned long physCol = mNumCols == 4ul ? 3ul : 2ul;

  auto gzFile = gzopen(mInputFile.string().c_str(), "r");

  while (!gzeof(gzFile)) {
    std::vector<std::string> line = splitTextByDelimiter(readNextLineFromGzip(gzFile), "\t");
    if (!line.empty()) {

      if (line.size() != mNumCols) {
        throw std::runtime_error(
            fmt::format("Error: PLINK map file {} line {} contains {} columns, but line 1 contains {} columns\n",
                        mInputFile.string(), 1ul + mChrIds.size(), line.size(), mNumCols));
      }

      mChrIds.emplace_back(line.at(chrCol));
      mSnpIds.emplace_back(line.at(snpCol));
      if (mNumCols == 4ul) {
        mGeneticPositions.emplace_back(std::stod(line.at(genCol)));
      }

      try {
        mPhysicalPositions.emplace_back(ulFromString(line.at(physCol)));
      } catch (const std::runtime_error& e) {
        gzclose(gzFile);
        throw std::runtime_error(
            fmt::format("Error: PLINK map file {} line {} column {}: expected unsigned integer but got {}\n",
                        mInputFile.string(), 1ul + mPhysicalPositions.size(), 1ul + physCol, line.at(physCol)));
      }
    }
  }

  gzclose(gzFile);
}
unsigned long PlinkMap::getNumSites() const {
  return mNumSites;
}
unsigned long PlinkMap::getNumCols() const {
  return mNumCols;
}
const std::vector<std::string>& PlinkMap::getChrIds() const {
  return mChrIds;
}
const std::vector<std::string>& PlinkMap::getSnpIds() const {
  return mSnpIds;
}
const std::vector<double>& PlinkMap::getGeneticPositions() const {
  return mGeneticPositions;
}
const std::vector<unsigned long>& PlinkMap::getPhysicalPositions() const {
  return mPhysicalPositions;
}

} // namespace asmc
