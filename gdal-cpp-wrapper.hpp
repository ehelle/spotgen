#ifndef GDAL_CPP_WRAPPER_HPP
#define GDAL_CPP_WRAPPER_HPP

#include <string>


#include "gdal_priv.h"
#include "cpl_conv.h"

#include <Eigen/Dense>

using Mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

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
      poBand = poDataset->GetRasterBand(1);
      poDataset->GetGeoTransform(gt);
      ncol = poDataset->GetRasterXSize();
      nrow = poDataset->GetRasterYSize();
      pixelSizeX = gt[1];
      pixelSizeY = gt[5];
      
    }

    ~dataset() {
      GDALClose((GDALDataset*) poDataset);
    }

    auto gdal2eigen(int ulx, int uly, int lrx, int lry) -> Mat;

    auto gdal2eigen() -> Mat { return gdal2eigen(0, 0, ncol, nrow); }

    auto readline(double* pdfRowM, int nXOff, int nYOff, int width) -> void;
  
    
  private:
    GDALDataset *poDataset;
    GDALRasterBand *poBand;
    double gt[6]; // geotransform
    int ncol, nrow;
    double pixelSizeX, pixelSizeY;
    
  }; // class dataset
    
  
} // namespace gdal

#endif
