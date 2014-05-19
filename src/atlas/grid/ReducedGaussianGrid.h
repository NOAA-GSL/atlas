#ifndef atlas_reduced_gaussian_grid_H
#define atlas_reduced_gaussian_grid_H
/*
 * (C) Copyright 1996-2014 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <cstddef>
#include <vector>

#include "atlas/grid/GribGrid.h"

//-----------------------------------------------------------------------------

namespace atlas {
namespace grid {

//-----------------------------------------------------------------------------

// A gaussian grid is a latitude/longitude grid.
// The spacing of the latitudes is not regular.
// However, the spacing of the lines of latitude is symmetrical about the Equator.
// Note that there is no latitude at either Pole or at the Equator.
// A grid is usually referred to by its 'number' N, which is the number of lines of latitude between a Pole and the Equator.
// The longitudes of the grid points are defined by giving the number of points along each line of latitude.
// The first point is at longitude 0 and the points are equally spaced along the line of latitude.
// In a regular Gaussian grid, the number of longitude points along a latitude is 4*N.
// In a reduced Gaussian grid, the number of longitude points along a latitude is specified.
// Latitudes may have differing numbers of points but the grid is symmetrical about the Equator.
// A reduced gaussian grid may also be called a quasi-regular Gaussian grid.
//
// ? Global regular grids generated by interpolation have a longitude at 0 deg West.
// Latitude/longitude grids contain a line of latitude at the Equator, and gaussian grids are symmetrical about the Equator.

//
// The Grib consist of sections  Geom, Bitmap(optional), Data
// We have following keys: numberofDataPoints,numberOfMissing,numberOf_CodedValues
// o When BitMap is present,(we have and array), it is for the global, 0 - Missing,  1- data value present
// o The bitsmap shows where we have Data values.

class ReducedGaussianGrid : public GribGrid {
public:
   ReducedGaussianGrid( grib_handle* h );
   virtual ~ReducedGaussianGrid();

   /// Overridden functions
   virtual const char* gridType() const { return "reduced_gg"; }
   virtual BoundBox boundingBox() const;
   virtual size_t nPoints() const { return points_.size(); }
   virtual void coordinates( Grid::Coords & ) const;
   /// @deprecated will be removed soon as it exposes the inner storage of the coordinates
   virtual const std::vector<Point>& coordinates() const { return points_; }

   /// Functions specific to Reduced Guassian grids
   long gaussianNumber() const { return gaussianNumber_;}

private:
   void add_point(int lat_index);

   // TODO common code
   bool isGlobalNorthSouth() const;
   bool isGlobalWestEast() const;

private:
   long   gaussianNumber_;          /// No of points between pole and equator
   long   nj_;                      /// No of points along Y axes
   std::vector<long> rgSpec_;
   std::vector< Point > points_;     ///< storage of coordinate points
   std::vector<double> latitudes_;
};

//-----------------------------------------------------------------------------

} // namespace grid
} // namespace eckit

#endif
