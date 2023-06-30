#pragma once

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcppsimple/ParameterSet.h"
#include "fhiclcppsimple/make_ParameterSet.h"

namespace nusyst{

  fhiclsimple::ParameterSet FhiclToFhiclSimple(const fhicl::ParameterSet& f);
  fhicl::ParameterSet FhiclSimpleToFhicl(const fhiclsimple::ParameterSet& fs);

} // end namespace nusyst
