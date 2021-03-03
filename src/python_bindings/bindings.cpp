#include <pybind11/pybind11.h>

#include "BedMatrixType.hpp"
#include "HapsMatrixType.hpp"

#include "utils/StringUtils.hpp"

namespace py = pybind11;

PYBIND11_MODULE(asmc_data_module, m) {

  m.def("stripBack", &asmc::stripBack);

  py::class_<asmc::HapsMatrixType>(m, "HapsMatrixType")
      .def_static("createFromHapsPlusSamples", &asmc::HapsMatrixType::createFromHapsPlusSamples);

  py::class_<asmc::BedMatrixType>(m, "BedMatrixType")
      .def_static("createFromBedBimFam", &asmc::BedMatrixType::createFromBedBimFam)
      .def("writeFrequencies", &asmc::BedMatrixType::writeFrequencies)
      .def("printTiming", &asmc::BedMatrixType::printTiming);
}