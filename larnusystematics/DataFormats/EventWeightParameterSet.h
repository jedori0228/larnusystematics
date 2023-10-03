#ifndef _LARNUSYST_EVENTWEIGHTPARAMETERSET_H_
#define _LARNUSYST_EVENTWEIGHTPARAMETERSET_H_

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
    kDefault = -1,
    kMultisim = 0,
    kPMNSigma = 1,
    kFixed = 2,
    kMultisigma = 3,
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

