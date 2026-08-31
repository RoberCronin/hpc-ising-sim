#include "engine.hpp"
#include <cstddef>
#include <nanobind/ndarray.h>

namespace nb = nanobind;

IsingEngine::IsingEngine(int size, double temp)
    : size(size), temp(temp), lattice(size * size, 1), uniform_dist(0.0, 1.0) {
  std::random_device rd;
  rng.seed(rd());

  // initial states start with 50%/50% chance of 1/-1
  for (int i = 0; i < size * size; ++i) {
    lattice[i] = (uniform_dist(rng) > 0.5) ? 1 : -1;
  }
}

void IsingEngine::step_metropolis(size_t steps) {
  for (size_t i = 0; i < steps; i++) {
    int idx = rng() % (size * size);
    int x = idx % size;
    int y = idx / size;

    // find neighbors with periodic boundary conditions
    // we add size in case we are at x=0
    int left = get_index((x - 1 + size) % size, y);
    int right = get_index((x + 1) % size, y);
    // we add size in case we are at y=0
    int up = get_index(x, (y - 1 + size) % size);
    int down = get_index(x, (y + 1) % size);

    int neighbor_sum =
        lattice[left] + lattice[right] + lattice[up] + lattice[down];
    int delta_energy = 2 * lattice[idx] * neighbor_sum;

    if (delta_energy <= 0) {
      lattice[idx] *= -1;
    } else if (uniform_dist(rng) < std::exp(-delta_energy / temp)) {
      lattice[idx] *= -1;
    }
  }
}

nb::ndarray<nb::numpy, int32_t, nb::ndim<2>> IsingEngine::get_lattice_view() {
  size_t shape[2] = {size, size};
  size_t dimensions = 2;

  return nb::ndarray<nb::numpy, int32_t, nb::ndim<2>>(
      get_lattice_data().data(), dimensions, shape, nb::handle(), nullptr,
      nb::dtype<int32_t>());
}
