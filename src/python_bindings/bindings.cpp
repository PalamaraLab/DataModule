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
      .def("getDataAsFloat", &asmc::HapsMatrixType::getDataAsFloat)
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
      .def("getNumIndividuals", &asmc::BedMatrixType::getNumIndividuals)
      .def("getNumSites", &asmc::BedMatrixType::getNumSites)
      .def("getPhysicalPositions", &asmc::BedMatrixType::getPhysicalPositions)
      .def("getGeneticPositions", &asmc::BedMatrixType::getGeneticPositions)
      .def("getSiteNames", &asmc::BedMatrixType::getSiteNames)
      .def("getData", &asmc::BedMatrixType::getData)
      .def("getDataAsFloat", &asmc::BedMatrixType::getDataAsFloat)
      .def("getSite", &asmc::BedMatrixType::getSite)
      .def("getIndividual", &asmc::BedMatrixType::getIndividual)
      .def("getMissingCount", &asmc::BedMatrixType::getMissingCount)
      .def("getMissingCounts", &asmc::BedMatrixType::getMissingCounts)
      .def("getMissingFrequency", &asmc::BedMatrixType::getMissingFrequency)
      .def("getMissingFrequencies", &asmc::BedMatrixType::getMissingFrequencies)
      .def("getMinorAlleleCount", &asmc::BedMatrixType::getMinorAlleleCount)
      .def("getDerivedAlleleCount", &asmc::BedMatrixType::getDerivedAlleleCount)
      .def("getMinorAlleleCounts", &asmc::BedMatrixType::getMinorAlleleCounts)
      .def("getDerivedAlleleCounts", &asmc::BedMatrixType::getDerivedAlleleCounts)
      .def("getMinorAlleleFrequency", &asmc::BedMatrixType::getMinorAlleleFrequency)
      .def("getDerivedAlleleFrequency", &asmc::BedMatrixType::getDerivedAlleleFrequency)
      .def("getMinorAlleleFrequencies", &asmc::BedMatrixType::getMinorAlleleFrequencies)
      .def("getDerivedAlleleFrequencies", &asmc::BedMatrixType::getDerivedAlleleFrequencies)
      .def("writeFrequencies", &asmc::BedMatrixType::writeFrequencies)
      .def("printTiming", &asmc::BedMatrixType::printTiming);

}