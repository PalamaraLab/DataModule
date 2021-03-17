// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "BedMatrixType.hpp"

extern "C" {
#include "third_party/pandas_plink/bed_reader.h"
}

#include "utils/FileUtils.hpp"
#include "utils/StringUtils.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <string_view>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>

namespace asmc {

BedMatrixType BedMatrixType::createFromBedBimFam(std::string_view bedFile, std::string_view bimFile,
                                                 std::string_view famFile) {
  if (!fs::exists(bedFile) || !fs::is_regular_file(bedFile)) {
    throw std::runtime_error(fmt::format("Expected .bed file, but got {}", bedFile));
  }
  if (!fs::exists(bimFile) || !fs::is_regular_file(bimFile)) {
    throw std::runtime_error(fmt::format("Expected .bim file, but got {}", bimFile));
  }
  if (!fs::exists(famFile) || !fs::is_regular_file(famFile)) {
    throw std::runtime_error(fmt::format("Expected .fam file, but got {}", famFile));
  }

  BedMatrixType instance;
  instance.readBimFile(bimFile);
  instance.readFamFile(famFile);
  instance.readBedFile(bedFile);

  return instance;
}

void BedMatrixType::determineFamDelimiter(const fs::path& famFile) {
  auto gzFile = gzopen(famFile.string().c_str(), "r");
  auto firstLine = readNextLineFromGzip(gzFile);

  std::vector<std::string> delimiters = {" ", "\t"};
  for (const auto& delimiter : delimiters) {
    if (splitTextByDelimiter(firstLine, delimiter).size() == 6ul) {
      mFamDelimeter = delimiter;
      gzclose(gzFile);
      return;
    }
  }
  gzclose(gzFile);
  throw std::runtime_error(fmt::format("Could not determine delimiter for .fam file {}", famFile.string()));
}

void BedMatrixType::readBedFile(const fs::path& bedFile) {
  mData.resize(static_cast<index_t>(getNumIndividuals()), static_cast<index_t>(getNumSites()));

  const auto nRows = static_cast<uint64_t>(getNumSites());
  const auto nCols = static_cast<uint64_t>(getNumIndividuals());
  const auto rowStart = static_cast<uint64_t>(0ul);
  const auto colStart = static_cast<uint64_t>(0ul);
  const auto rowEnd = nRows;
  const auto colEnd = nCols;
  std::array<uint64_t, 2> strides = {static_cast<uint64_t>(mData.colStride()),
                                     static_cast<uint64_t>(mData.rowStride())};

  read_bed_chunk(bedFile.string().data(), nRows, nCols, rowStart, colStart, rowEnd, colEnd, mData.data(),
                 strides.data());
  mMissingCounts = (mData.array() == static_cast<uint8_t>(mMissingInt)).colwise().count().cast<unsigned long>();
}

void BedMatrixType::readBimFile(const fs::path& bimFile) {
  auto gzFile = gzopen(bimFile.string().c_str(), "r");

  while (!gzeof(gzFile)) {
    std::vector<std::string> line = splitTextByDelimiter(readNextLineFromGzip(gzFile), "\t");
    if (!line.empty()) {
      assert(line.size() == 6ul);

      mSiteNames.emplace_back(line.at(1));
      mGeneticPositions.emplace_back(std::stod(line.at(2)));
      mPhysicalPositions.emplace_back(std::stoul(line.at(3)));
    }
  }

  gzclose(gzFile);
}

void BedMatrixType::readFamFile(const fs::path& famFile) {
  determineFamDelimiter(famFile);
  auto gzFile = gzopen(famFile.string().c_str(), "r");

  unsigned long numIndividuals = 0ul;
  while (!gzeof(gzFile)) {
    std::vector<std::string> line = splitTextByDelimiter(readNextLineFromGzip(gzFile), mFamDelimeter);
    if (!line.empty()) {
      assert(line.size() == 6ul);
      numIndividuals++;
    }
  }
  mNumIndividuals = numIndividuals;
  gzclose(gzFile);
}

unsigned long BedMatrixType::getAlleleCount(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return mData.col(static_cast<index_t>(siteId)).cast<unsigned long>().sum() -
         static_cast<unsigned long>(mMissingInt) * getMissingCount(siteId);
}

rvec_ul_t BedMatrixType::getAlleleCounts() const {
  return mData.cast<unsigned long>().colwise().sum().array() -
         (static_cast<unsigned long>(mMissingInt) * mMissingCounts).array();
}

unsigned long BedMatrixType::getNumIndividuals() const {
  return mNumIndividuals;
}

unsigned long BedMatrixType::getNumSites() const {
  return static_cast<unsigned long>(mGeneticPositions.size());
}

const std::vector<unsigned long>& BedMatrixType::getPhysicalPositions() const {
  return mPhysicalPositions;
}

const std::vector<double>& BedMatrixType::getGeneticPositions() const {
  return mGeneticPositions;
}

const mat_uint8_t& BedMatrixType::getData() const {
  return mData;
}

mat_float_t BedMatrixType::getDataAsFloat() const {
  return (mData.cast<float>().array() == static_cast<float>(mMissingInt)).select(mMissingFloat, mData.cast<float>());
}

rvec_uint8_t BedMatrixType::getIndividual(unsigned long individualId) const {
  assert(individualId < getNumIndividuals());
  return mData.row(static_cast<index_t>(individualId));
}

cvec_uint8_t BedMatrixType::getSite(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return mData.col(static_cast<index_t>(siteId));
}

const std::vector<std::string>& BedMatrixType::getSiteNames() const {
  return mSiteNames;
}

unsigned long BedMatrixType::getMissingCount(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return mMissingCounts(static_cast<index_t>(siteId));
}

rvec_ul_t BedMatrixType::getMissingCounts() const {
  return mMissingCounts;
}

double BedMatrixType::getMissingFrequency(unsigned long siteId) const {
  return static_cast<double>(getMissingCount(siteId)) / static_cast<double>(getNumIndividuals());
}

rvec_dbl_t BedMatrixType::getMissingFrequencies() const {
  return mMissingCounts.cast<double>().array() / static_cast<double>(getNumIndividuals());
}

unsigned long BedMatrixType::getMinorAlleleCount(unsigned long siteId) const {
  assert(siteId < getNumSites());
  const unsigned long rawCount = getAlleleCount(siteId);
  const unsigned long notMissing = getNumIndividuals() - getMissingCount(siteId);
  return rawCount <= notMissing ? rawCount : 2ul * notMissing - rawCount;
}

rvec_ul_t BedMatrixType::getMinorAlleleCounts() const {
  rvec_ul_t rawCounts = getAlleleCounts();
  rvec_ul_t threshold = getNumIndividuals() - getMissingCounts().array();
  return (rawCounts.array() > threshold.array()).select(2ul * threshold.array() - rawCounts.array(), rawCounts);
}

unsigned long BedMatrixType::getDerivedAlleleCount(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return getAlleleCount(siteId);
}

rvec_ul_t BedMatrixType::getDerivedAlleleCounts() const {
  return getAlleleCounts();
}

double BedMatrixType::getMinorAlleleFrequency(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return static_cast<double>(getMinorAlleleCount(siteId)) /
         (2.0 * static_cast<double>(getNumIndividuals() - getMissingCount(siteId)));
}

double BedMatrixType::getDerivedAlleleFrequency(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return static_cast<double>(getDerivedAlleleCount(siteId)) /
         (2.0 * static_cast<double>(getNumIndividuals() - getMissingCount(siteId)));
}

cvec_dbl_t BedMatrixType::getMinorAlleleFrequencies() const {
  return getMinorAlleleCounts().cast<double>().array() /
         (2.0 * (getNumIndividuals() - getMissingCounts().array()).cast<double>());
}

cvec_dbl_t BedMatrixType::getDerivedAlleleFrequencies() const {
  return getDerivedAlleleCounts().cast<double>().array() /
         (2.0 * (getNumIndividuals() - getMissingCounts().array()).cast<double>());
}

void BedMatrixType::writeFrequencies(std::string_view frqFile) {

  rvec_dbl_t freq = getMinorAlleleFrequencies();
  rvec_ul_t NCHROBS = 2ul * (getNumIndividuals() - mMissingCounts.array());

  FILE* fp = std::fopen(frqFile.data(), "w");
  fmt::print(fp, " CHR           SNP   A1   A2          MAF  NCHROBS\n", freq[0], NCHROBS[0]);

  for (unsigned long i = 0ul; i < getNumSites(); ++i) {
    auto ii = static_cast<index_t>(i);
    fmt::print(fp, "{:>4}{:>14}{:>5}{:>5}{:>13}{:>9}\n", 1, mSiteNames.at(i), 1, 2, freq[ii], NCHROBS[ii]);
  }

  std::fclose(fp);
}

} // namespace asmc
