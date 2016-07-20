#include "gdal-cpp-wrapper.hpp"
#include <cassert>
#include <iostream>

namespace gdal {
    
  auto dataset::gdal2eigen(int ulx, int uly, int lrx, int lry) -> Eigen::MatrixXd {
    int width(lrx - ulx);
    int height(uly - lry);
    //    std::assert(width > 0 && width < ncol && height > 0 && height < nrow);
    std::cout << ulx << " " << uly << std::endl;
    Eigen::MatrixXd m(1,1);
    m(0,0) = 5;
    return m;
  }

} // namespace gdal
