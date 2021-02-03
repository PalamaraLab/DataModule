// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_EIGENTYPES_HPP
#define DATA_MODULE_EIGENTYPES_HPP

#include <Eigen/Core>

namespace asmc {

using mat_bool_t = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;
using index_t = Eigen::Index;

using rvec_bool_t = Eigen::Matrix<bool, 1, Eigen::Dynamic>;
using cvec_bool_t = Eigen::Matrix<bool, Eigen::Dynamic, 1>;

} // namespace asmc

#endif // DATA_MODULE_EIGENTYPES_HPP
