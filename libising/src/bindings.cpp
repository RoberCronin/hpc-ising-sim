#include "engine.hpp"
#include <nanobind/nanobind.h>

namespace nb = nanobind;

NB_MODULE(_libising_impl, m) {
  nb::class_<IsingEngine>(m, "IsingEngine")
      .def(nb::init<int, double>(), nb::arg("size"), nb::arg("temp"))
      .def("step_metropolis", &IsingEngine::step_metropolis, nb::arg("steps"))
      .def("get_lattice_view", &IsingEngine::get_lattice_view,
           nb::rv_policy::reference_internal);
}
