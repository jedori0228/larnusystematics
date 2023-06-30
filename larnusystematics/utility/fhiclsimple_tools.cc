#include "larnusystematics/utility/fhiclsimple_tools.h"

fhiclsimple::ParameterSet nusyst::FhiclToFhiclSimple(const fhicl::ParameterSet& f) {
  return fhiclsimple::make_ParameterSet_from_string( f.to_indented_string() );
}

fhicl::ParameterSet nusyst::FhiclSimpleToFhicl(const fhiclsimple::ParameterSet& fs){
  return fhicl::ParameterSet::make( fs.to_indented_string() );
}

