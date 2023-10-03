#include <cassert>
#include <string>
#include <map>
#include <vector>
#include "larnusystematics/DataFormats/EventWeightParameterSet.h"

namespace larnusyst {
  namespace evwgh {

void EventWeightParameterSet::AddParameter(std::string name, double cv, std::vector<double> vals){
  fEVParameters.emplace_back( name, cv, vals );
}

  }  // namespace evwgh
}  // namespace larnusyst

