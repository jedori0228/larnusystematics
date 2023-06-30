#include "larnusystematics/utility/fhiclsimple_tools.h"

fhiclsimple::ParameterSet nusyst::FhiclToFhiclSimple(const fhicl::ParameterSet& f) {
  std::string str_f = f.to_indented_string();
  return fhiclsimple::make_ParameterSet_from_string(str_f);
}

fhicl::ParameterSet nusyst::FhiclSimpleToFhicl(const fhiclsimple::ParameterSet& fs){
  return fhicl::ParameterSet::make( fs.to_indented_string() );
}

