#ifndef _LARNUSYST_EVENTWEIGHTPARAMETERSET_H_
#define _LARNUSYST_EVENTWEIGHTPARAMETERSET_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>

namespace larnusyst {
  namespace evwgh {

struct EventWeightParameter {

  EventWeightParameter() : fName(""), fCentralValue(0.), fParamVariations{}
  {
  }

  EventWeightParameter(std::string name, double cv, std::vector<double> vals)
      : fName(name), fCentralValue(cv), fParamVariations(vals)
  {
  }

  /** Comparison operator (required for use as an std::map key). */
  inline friend bool operator<(const EventWeightParameter& lhs,
                               const EventWeightParameter& rhs) {
    return lhs.fName < rhs.fName;
  }

  /** Equality operator, testing equality of all members. */
  inline friend bool operator==(const EventWeightParameter& lhs,
                                const EventWeightParameter& rhs) {
    return (lhs.fName == rhs.fName &&
            lhs.fCentralValue == rhs.fCentralValue &&
            lhs.fParamVariations == rhs.fParamVariations);
  }

  std::string fName;
  double fCentralValue;
  std::vector<double> fParamVariations;
};

class EventWeightParameterSet {
public:

  typedef enum rwtype
  {
    kDefaultRWType = -1,
    kMultisim = 0,
    kMultisigma = 1,
  } ReweightType;

  EventWeightParameterSet(){}
  EventWeightParameterSet(std::string name, ReweightType rwtype, size_t NUniv):
  fName(name), fRWType(rwtype), fNuniverses(NUniv)
  {
  }

  void SetName(std::string name){ fName=name; }
  void SetRWType(ReweightType rwtype){ fRWType=rwtype; }
  void SetNUniverses(size_t NUniv){ fNuniverses=NUniv; }

  /** Equality operator, testing equality of all members. */
  inline friend bool operator==(const EventWeightParameterSet& lhs,
                                const EventWeightParameterSet& rhs) {
    return (lhs.fName == rhs.fName &&
            lhs.fRWType == rhs.fRWType &&
            lhs.fNuniverses == rhs.fNuniverses &&
            lhs.fEVParameters == rhs.fEVParameters);

  }

  // Overload the << operator for your class
  friend std::ostream& operator<<(std::ostream& os, const EventWeightParameterSet& p) {
    os << "ParameterSet" << "\n" << "- Name: " << p.fName << "\n" << "- RWType: " << p.fRWType << std::endl;
    os << "- Parameters:" << std::endl;
    for(const auto& it: p.fEVParameters){
      os << "  - Name: " << it.fName << "\n" << "    - CV: " << it.fCentralValue << std::endl;
      os << "    - Variations: ";
      for(double v: it.fParamVariations) os << " " << v;
      os << std::endl;
    }
    return os;
  }

  void AddParameter(std::string name, double cv, std::vector<double> vals);

public:
  std::string fName;
  ReweightType fRWType;
  size_t fNuniverses;
  std::vector<EventWeightParameter> fEVParameters;
};

  }  // namespace evwgh
}  // namespace larnusyst

#endif  // _LARNUSYST_EVENTWEIGHTPARAMETERSET_H_

