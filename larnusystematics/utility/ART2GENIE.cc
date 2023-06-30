#include "larnusystematics/utility/ART2GENIE.h"

namespace nusyst{

  std::vector<std::unique_ptr<genie::EventRecord>> GetGENIERecords(
    const art::Handle<std::vector<simb::MCTruth>>& mcTruthHandle,
    const art::Handle<std::vector<simb::GTruth>>& gTruthHandle
  ){

    std::vector<std::unique_ptr<genie::EventRecord>> gheps;

    size_t NEventUnits = mcTruthHandle->size();
    if (mcTruthHandle->size() != gTruthHandle->size()) {
      NEventUnits = std::min(mcTruthHandle->size(), gTruthHandle->size());
    }
    for (size_t eu_it = 0; eu_it < NEventUnits; ++eu_it) {
      gheps.emplace_back(evgb::RetrieveGHEP(mcTruthHandle->at(eu_it),
                                            gTruthHandle->at(eu_it)));
    }

    return gheps;

  }

  std::vector<std::unique_ptr<genie::EventRecord>> GetGENIERecords(
    const art::Event& e,
    std::string mcTruthLabel,
    std::string gTruthLabel
  ){

    art::Handle<std::vector<simb::MCTruth>> mcTruthHandle = e.getHandle<std::vector<simb::MCTruth>>(mcTruthLabel);
    art::Handle<std::vector<simb::GTruth>> gTruthHandle = e.getHandle<std::vector<simb::GTruth>>(gTruthLabel);

    return GetGENIERecords(mcTruthHandle, gTruthHandle);


  }

} // end namespaace nusyst

