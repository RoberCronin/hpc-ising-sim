#pragma once

#include "BitLattice.hpp"
#include <cstddef>
#include <cstdint>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <random>

namespace nb = nanobind;

class IsingEngine {
private:
  size_t size; // width/height
  double temp; // using natural units (J), i.e. we dont need to use k_b
  BitLattice lattice;

  std::mt19937 rng;
  std::uniform_real_distribution<double> uniform_dist;

public:
  IsingEngine(int size, double temp);
  void step_metropolis(size_t steps);

  size_t get_size() const { return size; }

  nb::ndarray<nb::numpy, int32_t, nb::ndim<2>> get_unpacked_lattice();
};
