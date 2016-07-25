#include "spotfinder.hpp"
#include <iostream>

auto operator<(const xyh& lhs, const xyh& rhs) -> bool{
    return lhs.value > rhs.value;
}

auto spotfinder::is_local_maxima(int row, int col) -> bool {
  auto neighbours = get_neighbours(row, col);
  for (auto n : neighbours) {
    if (data(row, col) <= n.value) {
      return false;
    }
  }
  return true;
  
  //data(row,col) >= data.block(r-1, c-1, 3, 3).maxCoeff())
  /*
  if (data(row, col) <= data(row-1, col)) { return false; }
  else if (data(row, col) <= data(row+1, col)) { return false; }
  else if (data(row, col) <= data(row, col-1)) { return false; }
  else if (data(row, col) <= data(row, col+1)) { return false; }
  else { return true; };
  */
}
  
auto spotfinder::find_local_maxima() -> void {
  for (auto r = 1; r < rows-1; ++r) {
    for (auto c = 1; c < cols-1; ++c) {
      if (is_local_maxima(r,c)) {
	  local_maxima.push(xyh{r,c,data(r,c)});
      }
    }
  }
}

auto spotfinder::get_neighbours(int row, int col) -> std::vector<xyh> {
  auto res = std::vector<xyh>();
  if (row != 0) {
    res.push_back(xyh{ row-1, col, data(row-1, col) });
  }
  if (row != rows-1) {
    res.push_back(xyh{ row+1, col, data(row+1, col) });
  }
  if (cols != 0) {
    res.push_back(xyh{ row, col-1, data(row, col-1) });
  }
  if (cols != cols-1) {
    res.push_back(xyh{ row, col+1, data(row, col+1) });
  }
  return res;
}

auto spotfinder::find_valid_maxima(double margin) -> void {
  while (!local_maxima.empty()) {
    auto cell = local_maxima.top();
    local_maxima.pop();
    if (!is_flooded(cell.row, cell.col)) {
      std::cout << cell.row << " " << cell.col << " " << cell.value << std::endl;
      valid_maxima.push_back(cell);
      double until = cell.value - margin;
      std::cout << "test1";
      flood(cell.row, cell.col, until);
      std::cout << "test2\n";
    }
  }
  for (auto i : valid_maxima) {
    std::cout << i.value << std::endl;
  }
}

auto spotfinder::flood(int row, int col, double until) -> void {
  auto neighbours = get_neighbours(row, col);
  is_flooded(row, col) = true;
  for (auto n : neighbours) {
    if (!is_flooded(n.row, n.col) && n.value < until) {
      spotfinder::flood(n.row, n.col, until);
    }
  }
}
