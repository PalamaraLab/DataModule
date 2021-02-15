// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "BedMatrixType.hpp"

extern "C" {
#include "third_party/pandas_plink/bed_reader.h"
}


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
  instance.readBedFile(bedFile);

  return instance;
}

void BedMatrixType::readBedFile(const fs::path& bedFile) {

  mData.resize(static_cast<index_t>(10), static_cast<index_t>(200));
  std::array<uint64_t, 2> strides = {static_cast<uint64_t>(mData.rowStride()),
                                     static_cast<uint64_t>(mData.colStride())};

  read_bed_chunk(bedFile.string().data(), 1980837, 200, 0, 0, 10, 200, mData.data(), strides.data());
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

rvec_uint8_t BedMatrixType::getSite(unsigned long siteId) const {
  assert(siteId < getNumSites());
  return mData.row(static_cast<index_t>(siteId));
}

cvec_uint8_t BedMatrixType::getHap(unsigned long hapId) const {
  assert(hapId < 2ul * getNumIndividuals());
  return mData.col(static_cast<index_t>(hapId));
}

} // namespace asmc
