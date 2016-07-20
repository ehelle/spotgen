#ifndef GDAL_CPP_WRAPPER_HPP
#define GDAL_CPP_WRAPPER_HPP

#include <string>


#include "gdal_priv.h"
#include "cpl_conv.h"

#include <Eigen/Dense>

namespace gdal {

  struct init_object {
    init_object() { GDALAllRegister(); }
  };
  
  struct init_wrapper {
    init_wrapper() { static init_object init; }
  };

  class dataset : private init_wrapper {
    
  public:
    dataset(const std::string& filename) :
      poDataset((GDALDataset *) GDALOpen( filename.c_str(), GA_ReadOnly))
    {
      poDataset->GetGeoTransform(gt);
      ncol = poDataset->GetRasterXSize();
      nrow = poDataset->GetRasterYSize();
      pixelSizeX = gt[1];
      pixelSizeY = gt[5];
      
    }

    ~dataset() {
      GDALClose((GDALDataset*) poDataset);
    }

    double get_x() { return gt[1]; }

    auto gdal2eigen(int ulx, int uly, int lrx, int lry) -> Eigen::MatrixXd;
      
    
  private:
    GDALDataset *poDataset;
    double gt[6]; // geotransform
    int ncol, nrow;
    double pixelSizeX, pixelSizeY;
    
  }; // class dataset
    
  
} // namespace gdal

#endif
