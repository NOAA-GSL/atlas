/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef atlas_output_Output_h
#define atlas_output_Output_h

#include <iosfwd>
#include <string>
#include "eckit/memory/Owned.h"
#include "eckit/serialisation/FileStream.h"
#include "eckit/config/Parametrisation.h"
#include "atlas/util/Config.h"

namespace eckit {
  class Parametrisation;
  class PathName;
}

namespace atlas {
namespace mesh {
    class Mesh;
} }

namespace atlas {
namespace field {
  class Field;
  class FieldSet;
} }

namespace atlas {
namespace output {
  
typedef std::ostream Stream;
typedef eckit::PathName PathName;

// -----------------------------------------------------------------------------

class Output : public eckit::Owned {

public:

  typedef eckit::SharedPtr<Output> Ptr;
  typedef atlas::util::Config Parameters;
  static Output* create(const std::string&, Stream&, const eckit::Parametrisation & = util::NoConfig() );

public:

    Output();

    virtual ~Output();

    /// Write mesh file
    virtual void output(
        const mesh::Mesh&,
        const eckit::Parametrisation& = util::NoConfig() ) const = 0;

    /// Write field to file
    virtual void output(
        const field::Field&,
        const eckit::Parametrisation& = util::NoConfig() ) const = 0;

    /// Write fieldset to file using FunctionSpace
    virtual void output(
        const field::FieldSet&,
        const eckit::Parametrisation& = util::NoConfig() ) const = 0;

    /// Write field to file using Functionspace
    virtual void output(
        const field::Field&,
        const functionspace::FunctionSpace&,
        const eckit::Parametrisation& = util::NoConfig() ) const = 0;

    /// Write fieldset to file using FunctionSpace
    virtual void output(
        const field::FieldSet&,
        const functionspace::FunctionSpace&,
        const eckit::Parametrisation& = util::NoConfig() ) const = 0;

};



class OutputFactory {
  public:
    /*!
     * \brief build Output with factory key, and default options
     * \return mesh generator
     */
    static Output* build(const std::string&, Stream&);

    /*!
     * \brief build Output with factory key inside parametrisation,
     * and options specified in parametrisation as well
     * \return mesh generator
     */
    static Output* build(const std::string&, Stream&, const eckit::Parametrisation&);

    /*!
     * \brief list all registered mesh generators
     */
    static void list(std::ostream &);

  private:
    std::string name_;
    virtual Output* make(Stream&) = 0 ;
    virtual Output* make(Stream&, const eckit::Parametrisation&) = 0 ;

  protected:

    OutputFactory(const std::string&);
    virtual ~OutputFactory();

};


template<class T>
class OutputBuilder : public OutputFactory {
  virtual Output* make(Stream& stream) {
      return new T(stream);
  }
  virtual Output* make(Stream& stream, const eckit::Parametrisation& param) {
        return new T(stream,param);
  }
  public:
    OutputBuilder(const std::string& name) : OutputFactory(name) {}
};

// -----------------------------------------------------------------------------

#define Parametrisation eckit::Parametrisation
#define Mesh mesh::Mesh
#define FieldSet field::FieldSet
#define Field field::Field
#define FunctionSpace functionspace::FunctionSpace

extern "C" {
void atlas__Output__delete(Output* This);
Output* atlas__Output__create(const char* factory_key, Stream* stream, const Parametrisation* params);
void atlas__Output__output_mesh(const Output* This, const Mesh* mesh, const Parametrisation* params);
void atlas__Output__output_fieldset(const Output* This, const FieldSet* fieldset, const Parametrisation* params);
void atlas__Output__output_field(const Output* This, const Field* field, const Parametrisation* params);
void atlas__Output__output_fieldset_fs(const Output* This, const FieldSet* fieldset, const FunctionSpace* functionspace, const Parametrisation* params);
void atlas__Output__output_field_fs(const Output* This, const Field* field, const FunctionSpace* functionspace, const Parametrisation* params);
}

#undef FieldSet
#undef Field
#undef Mesh
#undef Parametrisation
#undef FunctionSpace

} // namespace output
} // namespace atlas

#endif
