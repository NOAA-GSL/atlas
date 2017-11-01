/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#pragma once

#include "atlas/array.h"

//------------------------------------------------------------------------------

namespace atlas {
namespace array {
namespace helpers {

//------------------------------------------------------------------------------

/// Helper to assign a value to an array or arrayview
template <typename Value, unsigned int Rank>
struct array_assigner;

//------------------------------------------------------------------------------

// Recursive function to apply value to every index
template <typename Value, unsigned int Rank, unsigned int Dim>
struct array_assigner_impl {
  template <typename ... DimIndex>
  static void apply(ArrayView<Value,Rank>& arr, Value value, DimIndex... idxs) {
    for(size_t i=0; i < arr.shape(Dim); ++i) {
      array_assigner_impl<Value, Rank, Dim+1>::apply( arr, value, idxs..., i );
    }
  }
};

// End of recursion when Dim == Rank
template <typename Value, unsigned int Rank>
struct array_assigner_impl<Value, Rank, Rank> {
  template <typename ... DimIndex>
  static void apply(ArrayView<Value,Rank>& arr, Value value, DimIndex... idxs) {
      arr(idxs...) = value;
  }
};

//------------------------------------------------------------------------------


template <typename Value, unsigned int Rank>
struct array_assigner {

  template <typename T>
  static void apply(Array& arr, Value value) {
    return apply( make_host_view<T,Rank>(arr), value );
  }

  static void apply(ArrayView<Value,Rank>& arr, Value value) {
    array_assigner_impl<Value,Rank,0u>::apply( arr, value );
    // Note: no need to apply variadic pack (idxs...)
  }

};

//------------------------------------------------------------------------------

} // namespace helpers
} // namespace array
} // namespace atlas
