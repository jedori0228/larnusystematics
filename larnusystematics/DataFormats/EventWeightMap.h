#ifndef _LARNUSYST_EVENTWEIGHTMAP_H_
#define _LARNUSYST_EVENTWEIGHTMAP_H_

#include <map>
#include <vector>
#include <string>

namespace larnusyst {
  namespace evwgh {

typedef std::map<std::string, std::vector<double> > EventWeightMap;

  }  // namespace evwgh
}  // namespace larnusyst

#endif  // _LARNUSYST_EVENTWEIGHTMAP_H_

