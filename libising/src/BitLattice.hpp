#pragma once
#include <cstdint>
#include <vector>

class BitLattice {
private:
  int bit_size;
  int size;
  std::vector<uint64_t> data;

public:
  BitLattice(int size);

  int get_block_index(int block_x, int y) const;
  int get_spin(int x, int y) const;
  int get_size() const;
  int get_real_size() const;

  void flip_spin(int x, int y);
  const std::vector<uint64_t> &get_raw_data() const;
};
