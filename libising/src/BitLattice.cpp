#include "BitLattice.hpp"
#include <stdexcept>

BitLattice::BitLattice(int size) : bit_size(size), size(size / 64) {
  if (size <= 0) {
    throw std::invalid_argument("Lattice dimensions must be greater than 0");
  }
  if (size % 64 != 0) {
    throw std::invalid_argument("Lattice size must be a multiple of 64");
  }

  // set all bits to 0
  data.resize(this->size * bit_size, 0ULL);
}

int BitLattice::get_block_index(int block_x, int y) const {
  return y * size + block_x;
}

int BitLattice::get_spin(int x, int y) const {
  // use periodic boundary conditions
  x = (x + bit_size) % bit_size;
  y = (y + bit_size) % bit_size;

  int block_x = x / 64;
  int bit_offset = x & 63;

  uint64_t mask = (1ULL << bit_offset);
  return ((data[get_block_index(block_x, y)] & mask) != 0) ? 1 : -1;
}

void BitLattice::flip_spin(int x, int y) {
  // use periodic boundary conditions
  x = (x + bit_size) % bit_size;
  y = (y + bit_size) % bit_size;

  int block_x = x / 64;
  int bit_offset = x & 63;

  data[get_block_index(block_x, y)] ^= (1ULL << bit_offset);
}

int BitLattice::get_size() const { return bit_size; }

int BitLattice::get_real_size() const { return size; }

const std::vector<uint64_t> &BitLattice::get_raw_data() const { return data; }
