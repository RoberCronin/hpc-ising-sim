#include "IsingEngine.hpp"
#include "BitLattice.hpp"
#include <cstddef>
#include <nanobind/ndarray.h>

namespace nb = nanobind;

IsingEngine::IsingEngine(int size, double temp)
    : size(size), temp(temp), lattice(size), uniform_dist(0.0, 1.0) {
  std::random_device rd;
  rng.seed(rd());

  for (int y = 0; y < size; y++) {
    for (int x = 0; x < size; x++) {
      if (uniform_dist(rng) > 0.5) {
        lattice.flip_spin(x, y);
      }
    }
  }
}

void IsingEngine::step_metropolis(size_t steps) {
  for (size_t i = 0; i < steps; i++) {
    int idx = rng() % (size * size);
    int x = idx % size;
    int y = idx / size;

    int left = lattice.get_spin(x - 1, y);
    int right = lattice.get_spin(x + 1, y);
    int up = lattice.get_spin(x, y - 1);
    int down = lattice.get_spin(x, y + 1);

    int neighbor_sum = left + right + up + down;

    int delta_energy = 2 * lattice.get_spin(x, y) * neighbor_sum;

    if (delta_energy <= 0) {
      lattice.flip_spin(x, y);
    } else if (uniform_dist(rng) < std::exp(-delta_energy / temp)) {
      lattice.flip_spin(x, y);
    }
  }
}

nb::ndarray<nb::numpy, int32_t, nb::ndim<2>>
IsingEngine::get_unpacked_lattice() {
  // we just make a new array to send to python. we also will give ownership of
  // it to python
  int32_t *buffer = new int32_t[size * size];

  for (size_t y = 0; y < size; y++) {
    for (size_t x = 0; x < size; x++) {
      buffer[y * size + x] = lattice.get_spin(x, y);
    }
  }

  size_t shape[2] = {size, size};
  return nb::ndarray<nb::numpy, int32_t, nb::ndim<2>>(buffer, 2, shape);
}
