// This file is part of https://github.com/PalamaraLab/DataModule which is released under the GPL-3.0 license.
// See accompanying LICENSE and COPYING for copyright notice and full details.

#ifndef DATA_MODULE_EIGENTYPES_HPP
#define DATA_MODULE_EIGENTYPES_HPP

#include <Eigen/Core>

namespace asmc {

//using array_dt = Eigen::ArrayXd;
//using array_it = Eigen::ArrayXi;
//using vec_dt = Eigen::VectorXd;
//using rowvec_dt = Eigen::RowVectorXf;
//using three_dt = Eigen::Matrix3d;
//using four_dt = Eigen::Matrix4d;
//using mat_dt = Eigen::MatrixXd;

using mat_bool_t = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;
using index_t = Eigen::Index;

} // namespace asmc

#endif // DATA_MODULE_EIGENTYPES_HPP
