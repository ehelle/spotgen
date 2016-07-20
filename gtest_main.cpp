#include "gdal-cpp-wrapper.hpp"
#include <iostream>

int main(int argc, char** argv) {
  auto file = "hillshade.tif";
  auto test = gdal::dataset(file);
  auto x = test.get_x();
  std::cout << "dette er en test " << x << std::endl;
  auto mat = test.gdal2eigen(4,5,6,7);
  return 0;
}
