#include "spotfinder.hpp"
#include <iostream>

auto operator<(const xyh& lhs, const xyh& rhs) -> bool{
    return lhs.value > rhs.value;
}

auto spotfinder::is_local_maxima(xyh cell) -> bool {
  auto neighbours = get_neighbours(cell);
  for (auto n : neighbours) {
    if (data(cell.row, cell.col) <= n.value) {
      return false;
    }
  }
  return true;
}
  
auto spotfinder::find_local_maxima() -> void {
  for (auto r = 1; r < rows-1; ++r) {
    for (auto c = 1; c < cols-1; ++c) {
      auto cell = xyh{r,c,data(r,c)};
      if (is_local_maxima(cell)) {
	  local_maxima.push(cell);
      }
    }
  }
}

auto spotfinder::get_neighbours(xyh cell) -> std::vector<xyh> {
  auto res = std::vector<xyh>();
  auto r = cell.row;
  auto c = cell.col;
    
  if (r != 0) {
    res.push_back(xyh{ r-1, c, data(r-1, c) });
  }
  if (r != rows-1) {
    res.push_back(xyh{ r+1, c, data(r+1, c) });
  }
  if (c != 0) {
    res.push_back(xyh{ r, c-1, data(r, c-1) });
  }
  if (c != cols-1) {
    res.push_back(xyh{ r, c+1, data(r, c+1) });
  }
  return res;
}

auto spotfinder::find_valid_maxima(double margin) -> void {
  while (!local_maxima.empty()) {
    auto cell = local_maxima.top();
    local_maxima.pop();
    if (!is_flooded(cell.row, cell.col)) {
      //std::cout << cell.row << " " << cell.col << " " << cell.value << std::endl;
      valid_maxima.push_back(cell);
      double until = cell.value - margin;
      //std::cout << "test1" << std::endl;
      spotfinder::flood(cell, until);
      //std::cout << "test2" << std::endl;
    }
  }
  std::cout << valid_maxima.size() << std::endl;
  /*for (auto i : valid_maxima) {
    std::cout << i.value << std::endl;
    }*/
}

auto spotfinder::flood(xyh cell, double until) -> void {
  is_flooded(cell.row, cell.col) = true;
  if (cell.value < until) {
    auto neighbours = get_neighbours(cell);
    for (auto n : neighbours) {
      if (!is_flooded(n.row, n.col) && n.value < until) {
	spotfinder::flood(n, until);
      }
    }
  }
}
