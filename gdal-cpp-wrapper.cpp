#include "gdal-cpp-wrapper.hpp"
#include "spotfinder.hpp"
#include <cassert>
#include <iostream>

namespace gdal {
    
  auto dataset::gdal2eigen(int ulx, int uly, int lrx, int lry)
    -> Mat {

    int width(lrx - ulx);
    int height(lry - uly);
    assert(width > 0 && width <= ncol && height > 0 && height <= nrow);
    std::cout << height << " " << width << std::endl;
    Mat m(height,width);
    // load rasterdata to matrix
    for (auto row = 0; row < height; ++row) {
      double* pdfRowM = m.data() + row * width;
      dataset::readline(pdfRowM, ulx, row, width);
    }
    return m;
  }

  auto dataset::readline(double* pdfRowM, int nXOff, int nYOff, int width)
    -> void {
    poBand->RasterIO(GF_Read, nXOff, nYOff, width, 1,
		     pdfRowM, width, 1, GDT_Float64, 0, 0);
  }

  auto ogrdata::add_field(const std::string name, OGRFieldType type)
    -> void {
    OGRFieldDefn oField(name, type);
    // TODO setwidth for string
    if( poLayer->CreateField( &oField ) != OGRERR_NONE ) {
      printf( "Creating Name field failed.\n" );
      exit( 1 ); // TODO exception handling ??
    }
  }

  auto ogrdata::add_xyh_data(const xyh point)
    -> void {
    auto poFeature = ogrfeature(point);
    if ( poLayer->CreateFeature( poFeature ) != OGRERR_NONE ) {
      printf( "Failed to create feature in shapefile.\n" );
      exit( 1 ); // TODO exception handling ??
    }
  }
 
  
} // namespace gdal
