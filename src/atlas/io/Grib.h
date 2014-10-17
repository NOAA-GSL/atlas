/*
 * (C) Copyright 1996-2014 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef atlas_io_Grib_h
#define atlas_io_Grib_h

#include "atlas/atlas_config.h"

#ifdef ECKIT_HAVE_GRIB

#include "eckit/grib/GribHandle.h"

#include "atlas/FieldSet.h"
#include "atlas/GridSpec.h"

//------------------------------------------------------------------------------------------------------

namespace eckit { class PathName; }
namespace eckit { class DataHandle; }

namespace atlas {
namespace io {

//------------------------------------------------------------------------------------------------------

class Grib {

public: // methods

	static Grid::Ptr create_grid( eckit::grib::GribHandle& );

	/// Given a Grid, this function will find the closest matching GRIB samples file.
	/// The cloned/new handle of the GRIB sample file is returned.
	/// If no match found a NULL handle is returned.
	static eckit::grib::GribHandle::Ptr create_handle( const Grid&, long edition = 0 );

	/// Given a GridSpec return closest grib samples file.
	/// If no match found returns an empty string
	static std::string grib_sample_file( const GridSpec&, long editionNumber );

	/// Helper function, used locally and in testing
	static void determine_grib_samples_dir(std::vector<std::string>& sample_paths);

	static void write( const atlas::FieldSet& fset, const eckit::PathName& opath  );

	static void write( const atlas::Field& fh, eckit::DataHandle& dh );

	static eckit::grib::GribHandle::Ptr write( const atlas::Field& fh );

	static void clone( const atlas::FieldSet& field, const eckit::PathName& src, const eckit::PathName& opath  );

private: // methods

	static void write( const atlas::Field& field, const eckit::PathName& opath  );

	static void clone( const atlas::Field& field, const eckit::PathName& gridsec, eckit::DataHandle& );

	static eckit::grib::GribHandle::Ptr clone(const Field& field, eckit::grib::GribHandle& gridsec );

	/// @todo this function is temporary, until we make an abstract interface to output to different formats
	///       we must learn more about NetCDF, etc...
	///
	static void write_gridspec_to_grib( const GridSpec&, eckit::grib::GribHandle& );

};

//---------------------------------------------------------------------------------------------------------

} // namespace io
} // namespace atlas

#endif // ECKIT_HAVE_GRIB

#endif // atlas_io_Grib_h

