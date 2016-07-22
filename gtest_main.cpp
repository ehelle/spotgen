#include "gdal-cpp-wrapper.hpp"
#include "spotfinder.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  auto file = "hillshade.tif";
  auto test = gdal::dataset(file);
  std::cout << "dette er en test " << std::endl;
  auto mat = test.gdal2eigen();
  cout << "Here is mat.sum():       " << mat.sum()       << endl;
  cout << "Here is mat.prod():      " << mat.prod()      << endl;
  cout << "Here is mat.mean():      " << mat.mean()      << endl;
  cout << "Here is mat.minCoeff():  " << mat.minCoeff()  << endl;
  cout << "Here is mat.maxCoeff():  " << mat.maxCoeff()  << endl;
  cout << "Here is mat.trace():     " << mat.trace()     << endl;
  cout << mat.block(1,1,3,3).maxCoeff() << endl;
  auto& mp = mat;
  auto points = spotfinder(mp);
  cout << "test" << endl;
  points.find_local_maxima();
  return 0;
}
