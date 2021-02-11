// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_EIGENTYPES_HPP
#define DATA_MODULE_EIGENTYPES_HPP

// Silence warnings related to Eigen on Windows (C4996)
#define _SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING

#include <cstdint>

#include <Eigen/Core>

namespace asmc {

using index_t = Eigen::Index;

using mat_bool_t = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;
using rvec_bool_t = Eigen::Matrix<bool, 1, Eigen::Dynamic>;
using cvec_bool_t = Eigen::Matrix<bool, Eigen::Dynamic, 1>;

using mat_int8_t = Eigen::Matrix<int8_t, Eigen::Dynamic, Eigen::Dynamic>;
using rvec_int8_t = Eigen::Matrix<int8_t, 1, Eigen::Dynamic>;
using cvec_int8_t = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>;

using mat_uint8_t = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>;
using rvec_uint8_t = Eigen::Matrix<uint8_t, 1, Eigen::Dynamic>;
using cvec_uint8_t = Eigen::Matrix<uint8_t, Eigen::Dynamic, 1>;

} // namespace asmc

#endif // DATA_MODULE_EIGENTYPES_HPP
