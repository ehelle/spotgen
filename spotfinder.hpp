#ifndef SPOTFINDER_HPP
#define SPOTFINDER_HPP

#include "gdal-cpp-wrapper.hpp"

using BoolMat = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

class spotfinder {

public:
  spotfinder(const Mat& m) : data(m) {
    //data = m;
    rows = data.rows();
    cols = data.cols();
    flooded(rows, cols);
    flooded.fill(false);
  };

  auto find_local_maxima() -> void;

private:
  const Mat& data;
  BoolMat flooded;
  int rows, cols;
  
}; // class spotfinder

#endif
