#ifndef GDAL_CPP_WRAPPER_HPP
#define GDAL_CPP_WRAPPER_HPP

#include <string>
#include <algorithm>

#include "gdal_priv.h"
#include "cpl_conv.h"
#include "ogrsf_frmts.h"

#include "spotfinder.hpp"

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
    
  class ogrdata : private init_wrapper {

  public:
    ogrdata(const std::string filename) :
      pszDriverName("ESRI Shapefile")
  {
    poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
    // if ( poDriver == nullptr ) { throw Error } TODO
    poDataset = poDriver->Create(filename + ".shp", 0 ,0, 0, GDT_Unknown, NULL);
    // if NULL
    poLayer = poDataset->CreateLayer(filename, NULL, wkbPoint, NULL);
    //if NULL
     
      
  }
    ~ogrdata() {
      GDALClose(poDataset);
    }

    auto add_field(const std::string name, OGRFieldType type) -> void;
    
    auto add_real_field(const std::string name) { add_field(name, OFTReal); }

    auto add_xyh_data(const xyh point) -> void;

    auto add_xyh_data(const std::vector<xyh> vec) -> void {
      std::for_each(vec.begin(), vec.end(), add_xyh_data());
    }

    class ogrfeature : private init_wrapper {

    public:
      ogrfeature(const xyh point):
	poFeature(OGRFeature::CreateFeature( poLayer->getLayerDefn() ))
      {
	poFeature->SetField( "Height", point.value);
	OGRPoint pt; // destroy ??
	pt.setX( point.row );
	pt.setY( point.col );
	poFeature->SetGeometry( &pt );
      }
	  

      ~ogrfeature() { OGRFeature::DestroyFeature( poFeature ); }

    private:
      OGRFeature *poFeature;
    }
    
  private:
    const char *pszDriverName;
    GDALDriver *poDriver;
    GDALDataset *poDataset;
    OGRLayer *poLayer;
  }

    
} // namespace gdal

#endif
