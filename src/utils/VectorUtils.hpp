// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_VECTOR_UTILS_HPP
#define DATA_MODULE_VECTOR_UTILS_HPP

#include <algorithm>
#include <vector>

namespace asmc {

/**
 * Determine whether a vector is strictly increasing.
 *
 * @tparam T the type over which the vector is templated
 * @param vec vector to test for monotonicity
 * @return whether the vector is strictly increasing
 */
template <typename T> bool isStrictlyIncreasing(std::vector<T> vec) {
  return std::is_sorted(vec.begin(), vec.end(), [](const T& a, const T& b) { return a <= b; });
}

} // namespace asmc

#endif // DATA_MODULE_VECTOR_UTILS_HPP
