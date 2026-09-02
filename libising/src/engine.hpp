#pragma once

#include <cstddef>
#include <cstdint>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <random>
#include <vector>

namespace nb = nanobind;

class IsingEngine {
private:
  size_t size; // width/height
  double temp; // using natural units (J), i.e. we dont need to use k_b
  std::vector<int32_t> lattice; // 1D array representing the 2D grid

  std::mt19937 rng;
  std::uniform_real_distribution<double> uniform_dist;

  uint32_t get_index(uint16_t x, uint16_t y) const;

public:
  IsingEngine(int size, double temp);
  void step_metropolis(size_t steps);

  std::vector<int32_t> &get_lattice_data() { return lattice; }
  size_t get_size() const { return size; }

  nb::ndarray<nb::numpy, int32_t, nb::ndim<2>> get_unpacked_lattice();
};
