#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BedMatrixType.hpp"
#include "HapsMatrixType.hpp"

#include "utils/StringUtils.hpp"

namespace py = pybind11;

PYBIND11_MODULE(asmc_data_module, m) {

  m.def("stripBack", &asmc::stripBack);

  py::class_<asmc::HapsMatrixType>(m, "HapsMatrixType")
      .def_static("createFromHapsPlusSamples", &asmc::HapsMatrixType::createFromHapsPlusSamples)
      .def("getNumIndividuals", &asmc::HapsMatrixType::getNumIndividuals)
      .def("getNumHaps", &asmc::HapsMatrixType::getNumHaps)
      .def("getNumSites", &asmc::HapsMatrixType::getNumSites)
      .def("getPhysicalPositions", &asmc::HapsMatrixType::getPhysicalPositions)
      .def("getGeneticPositions", &asmc::HapsMatrixType::getGeneticPositions)
      .def("getData", &asmc::HapsMatrixType::getData)
      .def("getSite", &asmc::HapsMatrixType::getSite)
      .def("getHap", &asmc::HapsMatrixType::getHap)
      .def("getIndividual", &asmc::HapsMatrixType::getIndividual)
      .def("getMinorAlleleCount", &asmc::HapsMatrixType::getMinorAlleleCount)
      .def("getDerivedAlleleCount", &asmc::HapsMatrixType::getDerivedAlleleCount)
      .def("getMinorAlleleCounts", &asmc::HapsMatrixType::getMinorAlleleCounts)
      .def("getDerivedAlleleCounts", &asmc::HapsMatrixType::getDerivedAlleleCounts)
      .def("getMinorAlleleFrequency", &asmc::HapsMatrixType::getMinorAlleleFrequency)
      .def("getDerivedAlleleFrequency", &asmc::HapsMatrixType::getDerivedAlleleFrequency)
      .def("getMinorAlleleFrequencies", &asmc::HapsMatrixType::getMinorAlleleFrequencies)
      .def("getDerivedAlleleFrequencies", &asmc::HapsMatrixType::getDerivedAlleleFrequencies)
      ;

  py::class_<asmc::BedMatrixType>(m, "BedMatrixType")
      .def_static("createFromBedBimFam", &asmc::BedMatrixType::createFromBedBimFam)
      .def("writeFrequencies", &asmc::BedMatrixType::writeFrequencies)
      .def("printTiming", &asmc::BedMatrixType::printTiming);
}