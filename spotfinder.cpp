#include "spotfinder.hpp"
#include <iostream>


  
auto spotfinder::find_local_maxima() -> void {
  for (auto r = 1; r < rows-1; ++r) {
    for (auto c = 1; c < cols-1; ++c) {
      std::cout << r << " " << c << std::endl;
      if (data(r,c) > data.block(r-1, c-1, 3, 3).maxCoeff()) {
	std::cout << data(r,c) << std::endl;
      }
    }
  }
}

  


