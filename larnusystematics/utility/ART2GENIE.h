#pragma once

#include <iostream>

// art
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
// nugen
#include "nugen/EventGeneratorBase/GENIE/GENIE2ART.h"
// nusim
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/GTruth.h"
// GENIE
#include "Framework/EventGen/EventRecord.h"

namespace nusyst{

  std::vector<std::unique_ptr<genie::EventRecord>> GetGENIERecords(
    const art::Handle<std::vector<simb::MCTruth>>& mcTruthHandle,
    const art::Handle<std::vector<simb::GTruth>>& gTruthHandle
  );

  std::vector<std::unique_ptr<genie::EventRecord>> GetGENIERecords(
    const art::Event& e,
    std::string mcTruthLabel,
    std::string gTruthLabel
  );

} // end namespace nusyst
