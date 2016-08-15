#ifndef SPOTFINDER_HPP
#define SPOTFINDER_HPP

#include "gdal-cpp-wrapper.hpp"
#include <queue>
#include <vector>

using BoolMat = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

struct xyh {
  int row;
  int col;
  double value;
};

bool operator<(const xyh& lhs, const xyh& rhs);

class spotfinder {

public:
  spotfinder(const Mat& m) : data(m) { /*
			     rows(m.rows()),
			     cols(m.rows()),
			     flooded(new BoolMat(m.rows(), m.cols())){ */
    //data = m;
    rows = data.rows();
    cols = data.cols();
    is_flooded.resize(rows,cols);
    is_flooded.fill(false);
  };

  auto is_local_maxima(xyh cell) -> bool;
    
  auto find_local_maxima() -> void;

  auto get_neighbours(xyh cell) -> std::vector<xyh>;

  auto find_valid_maxima(double margin) -> void;

  auto flood(xyh cell, double until) -> void;

  auto return_maxima() -> std::vector<xyh> { return valid_maxima; }
  
private:
  const Mat& data;
  BoolMat is_flooded;
  int rows, cols;
  std::priority_queue<xyh> local_maxima;
  std::vector<xyh> valid_maxima;
  
}; // class spotfinder

#endif
