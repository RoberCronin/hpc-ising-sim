#include <nanobind/nanobind.h>

NB_MODULE(_libising_impl, m) {
  m.def(
      "sanity_check", []() { return "Hello from C++!"; },
      "Returns the string 'Hello from C++!'");
}
