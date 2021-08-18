// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#include "utils/VectorUtils.hpp"

#include <catch2/catch.hpp>

#include <vector>

namespace asmc {

TEST_CASE("utils/VectorUtils: test isStrictlyIncreasing", "[utils/VectorUtils]") {

  SECTION("Integer vectors") {
    std::vector<unsigned long> goodVecA = {1ul, 2ul, 3ul};
    std::vector<unsigned long> goodVecB = {3ul};
    std::vector<unsigned long> goodVecC = {};

    CHECK(isStrictlyIncreasing(goodVecA));
    CHECK(isStrictlyIncreasing(goodVecB));
    CHECK(isStrictlyIncreasing(goodVecC));

    std::vector<unsigned long> badVecA = {1ul, 1ul};
    std::vector<unsigned long> badVecB = {0ul, 2ul, 1ul};

    CHECK(!isStrictlyIncreasing(badVecA));
    CHECK(!isStrictlyIncreasing(badVecB));
  }

  SECTION("Floating vectors") {
    std::vector<double> goodVecA = {1.0, 2.0, 3.0};
    std::vector<double> goodVecB = {4.56};
    std::vector<double> goodVecC = {};

    CHECK(isStrictlyIncreasing(goodVecA));
    CHECK(isStrictlyIncreasing(goodVecB));
    CHECK(isStrictlyIncreasing(goodVecC));

    std::vector<double> badVecA = {3.0, 2.0, 1.0};
    std::vector<double> badVecB = {1.0, 2.0, 2.0};

    CHECK(!isStrictlyIncreasing(badVecA));
    CHECK(!isStrictlyIncreasing(badVecB));
  }
}

TEST_CASE("utils/VectorUtils: test isIncreasing", "[utils/VectorUtils]") {

  SECTION("Integer vectors") {
    std::vector<unsigned long> goodVecA = {1ul, 2ul, 3ul};
    std::vector<unsigned long> goodVecB = {3ul};
    std::vector<unsigned long> goodVecC = {};
    std::vector<unsigned long> goodVecD = {1ul, 1ul};

    CHECK(isIncreasing(goodVecA));
    CHECK(isIncreasing(goodVecB));
    CHECK(isIncreasing(goodVecC));
    CHECK(isIncreasing(goodVecD));


    std::vector<unsigned long> badVecA = {0ul, 2ul, 1ul};

    CHECK(!isIncreasing(badVecA));
  }

  SECTION("Floating vectors") {
    std::vector<double> goodVecA = {1.0, 2.0, 3.0};
    std::vector<double> goodVecB = {4.56};
    std::vector<double> goodVecC = {};
    std::vector<double> goodVecD = {1.0, 2.0, 2.0};

    CHECK(isIncreasing(goodVecA));
    CHECK(isIncreasing(goodVecB));
    CHECK(isIncreasing(goodVecC));
    CHECK(isIncreasing(goodVecD));

    std::vector<double> badVecA = {3.0, 2.0, 1.0};

    CHECK(!isIncreasing(badVecA));
  }
}

} // namespace asmc
