#include "engine.hpp"
#include <cstddef>
#include <cstdint>
#include <immintrin.h>
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

uint32_t IsingEngine::get_index(uint16_t x, uint16_t y) const {
  // 0x55555555 is 01010101... in binary (even bits)
  // 0xAAAAAAAA is 10101010... in binary (odd bits)
  uint32_t x_spread = _pdep_u32(x, 0x55555555);
  uint32_t y_spread = _pdep_u32(y, 0xAAAAAAAA);

  return x_spread | y_spread;
}

nb::ndarray<nb::numpy, int32_t, nb::ndim<2>>
IsingEngine::get_unpacked_lattice() {
  // we just make a new array to send to python. we also will give ownership of
  // it to python
  int32_t *buffer = new int32_t[size * size];

  for (size_t y = 0; y < size; y++) {
    for (size_t x = 0; x < size; x++) {
      buffer[y * size + x] = lattice[get_index(x, y)];
    }
  }

  size_t shape[2] = {size, size};
  return nb::ndarray<nb::numpy, int32_t, nb::ndim<2>>(buffer, 2, shape);
}
