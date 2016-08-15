#include "gdal-cpp-wrapper.hpp"
#include "spotfinder.hpp"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  auto file = "n35.dt2";
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
  //  Mat mb = mat.block(0,0,4,4);
  auto& mp = mat;
  auto points = spotfinder(mp);
  cout << "test" << endl;
  points.find_local_maxima();
  points.find_valid_maxima(20);
  auto pts = points.return_maxima();
  auto shpfile = gdal::ogrdata("test1");
  shpfile.add_real_field("Height");
  shpfile.add_xyh_data(pts);
  return 0;
}
