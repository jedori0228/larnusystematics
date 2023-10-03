////////////////////////////////////////////////////////////////////////
// Class: NuSystEventWeight
// Module Type: producer
// File: NuSystEventWeight_module.cc
//
// Initiated by Jaesung Kim (jae.sung.kim.3426@gmail.com)
//
////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "art/Utilities/make_tool.h"
#include "canvas/Persistency/Common/Assns.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "larnusystematics/DataFormats/EventWeightMap.h"
#include "larnusystematics/DataFormats/EventWeightParameterSet.h"

#include "systematicstools/interface/ISystProviderTool.hh"
#include "systematicstools/interpreters/ParamHeaderHelper.hh"
#include "systematicstools/utility/ParameterAndProviderConfigurationUtility.hh"
#include "systematicstools/utility/exceptions.hh"

#include "nusystematics/utility/response_helper.hh"

#include "nugen/EventGeneratorBase/GENIE/GENIE2ART.h"
#include "Framework/EventGen/EventRecord.h"

#include "canvas/Persistency/Common/Assns.h"
#include "art/Framework/Principal/Run.h"

namespace larnusyst::evwgh {

class NuSystEventWeight : public art::EDProducer {
public:
  explicit NuSystEventWeight(fhicl::ParameterSet const& p);

  NuSystEventWeight(NuSystEventWeight const &) = delete;
  NuSystEventWeight(NuSystEventWeight &&) = delete;
  NuSystEventWeight& operator = (NuSystEventWeight const&) = delete;
  NuSystEventWeight& operator = (NuSystEventWeight&&) = delete;

private:
  void produce(art::Event& e) override;
  void beginRun(art::Run& run) override;

private:
  std::string fGenieModuleLabel;
  bool fAllowMissingTruth;
  bool fDebugMode;

  nusyst::response_helper fParamHeaderHelper;

};


NuSystEventWeight::NuSystEventWeight(fhicl::ParameterSet const& p)
  : EDProducer{p}
{

  fGenieModuleLabel = p.get<std::string>("GeneratorModuleLabel", "generator");
  fAllowMissingTruth = p.get<bool>("AllowMissingTruth");
  fDebugMode = p.get<bool>("DebugMode", false);

  fhicl::ParameterSet f_syst_provider_config = p.get<fhicl::ParameterSet>("generated_systematic_provider_configuration");

  MF_LOG_DEBUG("SystToolsEventWeight") << "Configuring ISystProvider";

  fParamHeaderHelper.LoadProvidersAndHeaders(f_syst_provider_config);

  MF_LOG_DEBUG("SystToolsEventWeight") << "ISystProvider is configuered";

  produces<std::vector<larnusyst::evwgh::EventWeightMap> >();
  produces<art::Assns<simb::MCTruth, larnusyst::evwgh::EventWeightMap> >();
  produces<std::vector<larnusyst::evwgh::EventWeightParameterSet>, art::InRun>();

}


void NuSystEventWeight::produce(art::Event& e) {

  auto mcwghvec = std::make_unique<std::vector<larnusyst::evwgh::EventWeightMap> >();
  auto wghassns = std::make_unique<art::Assns<simb::MCTruth, larnusyst::evwgh::EventWeightMap> >();

  art::PtrMaker<larnusyst::evwgh::EventWeightMap> makeWeightPtr(e);

  std::vector<art::Ptr<simb::MCTruth> > mclist;
  art::Handle<std::vector<simb::MCTruth>> mcTruthHandle= e.getHandle<std::vector<simb::MCTruth>>(fGenieModuleLabel);

  // TODO If we alread have genie::EventRecord, use that instead of making it through RetrieveGHEP
  std::vector<std::unique_ptr<genie::EventRecord>> gheps = evgb::RetrieveGHEPs(e, fGenieModuleLabel, fGenieModuleLabel);

  // Prooceed even with missing handle if we want to require the MCTruth to be
  // found, so that an exception will be thrown explaining the problem.
  if(mcTruthHandle.isValid() || !fAllowMissingTruth){

    art::fill_ptr_vector(mclist, mcTruthHandle);

    if(fDebugMode){
      std::cout << "[NuSystEventWeight::produce] mclist.size() = " << mclist.size() << "\n"
                << "[NuSystEventWeight::produce] fParamHeaderHelper.GetSystProvider().size() = " << fParamHeaderHelper.GetSystProvider().size() << std::endl;
    }

    for( auto &sp : fParamHeaderHelper.GetSystProvider() ){

      std::unique_ptr<systtools::EventResponse> syst_resp = sp->GetEventResponses(gheps);

      if( !syst_resp ){
        throw cet::exception{ "NuSystEventWeight" }
          << "Got nullptr systtools::EventResponse from provider "
          << sp->GetFullyQualifiedName() << "\n";
      }

      // syst_resp->size() is (Number of MCTruth)
      // Each index corresponds to each of MCTruth
      int nMCTruthIndex = 0;
      if(fDebugMode) std::cout << "[NuSystEventWeight::produce]   syst_resp.size() (= Number of MCTruth) of this SystProvider = " << syst_resp->size() << std::endl;

      // Looping over syst_resp is identical to looping over MCTruth
      for(systtools::event_unit_response_t const& resp: *syst_resp) {
        // resp.size() corresponds to number of knobs we altered;
        // e.g., MaCCQE, MaCCRES, MvCCRE -> resp.size() = 3
        if(fDebugMode){
          std::cout << "[NuSystEventWeight::produce]     sp->GetSystMetaData().size() (expected) = " << sp->GetSystMetaData().size() << "\n"
                    << "[NuSystEventWeight::produce]     resp.size() of this syst_resp (produced) = " << resp.size() << std::endl;
        }

        // Below check is not valid if there is a dependent dial
        /*
        if(sp->GetSystMetaData().size()!=resp.size()){
          throw cet::exception{ "NuSystEventWeight" } 
            << "sp->GetFullyQualifiedName() = " << sp->GetFullyQualifiedName() << "\n"
            << "We expect to have " << sp->GetSystMetaData().size() << " knobs for this SystProvider, but "
                    << resp.size() << " are produced. "
                    << "Probably this particular event is not relevant to this systematic variation.\n";
        }
        */

        larnusyst::evwgh::EventWeightMap mcwgh;

        for( auto const& r: resp){

          // responses.size() is the number of universes
          systtools::SystParamHeader const &sph = fParamHeaderHelper.GetHeader( r.pid );
          std::string prettyName = sph.prettyName;

          if(sph.isResponselessParam) continue;

          if(fDebugMode){
            std::cout << "[NuSystEventWeight::produce]       pid of this resp = " << r.pid << "\n"
                      << "[NuSystEventWeight::produce]       prettyName of this resp = " << prettyName << "\n"
                      << "[NuSystEventWeight::produce]       paramVariations.size() of this resp (expected) = " << sph.paramVariations.size() << "\n"
                      << "[NuSystEventWeight::produce]       responses.size() of this resp (produced) = " << r.responses.size() << std::endl;
          }

          if(sph.isCorrection && (r.responses.size()!=1)){
            throw cet::exception{ "NuSystEventWeight" }
              << "prettyName of this resp = " << prettyName << "\n"
              << "We expect to have 1 universes as it is a correction, but "
                      << r.responses.size() << " are produced\n";
          }
          if(!sph.isCorrection && sph.paramVariations.size()!=r.responses.size()){
            throw cet::exception{ "NuSystEventWeight" }
              << "prettyName of this resp = " << prettyName << "\n"
              << "We expect to have " << sph.paramVariations.size() << " universes, but "
                      << r.responses.size() << " are produced. "
                      << "Probably this particular event is not relevant to this systematic variation.\n";
          }

          // r.responses : std::vector<double>
          // std::map<std::string, std::vector<float> > EventWeightMap
          mcwgh[sp->GetFullyQualifiedName()+"_"+prettyName].assign(r.responses.cbegin(), r.responses.cend());

        } // END loop over knobs

        mcwghvec->push_back(std::move(mcwgh));
        art::Ptr<larnusyst::evwgh::EventWeightMap> wghPtr = makeWeightPtr(mcwghvec->size() - 1);
        wghassns->addSingle(mclist.at(nMCTruthIndex), wghPtr);

        nMCTruthIndex++;

      } // END loop over MCTruth particles

    } // END systprovider loop

  }

  e.put(std::move(mcwghvec));
  e.put(std::move(wghassns));

}


void NuSystEventWeight::beginRun(art::Run& run) {

  auto p = std::make_unique<std::vector<larnusyst::evwgh::EventWeightParameterSet> >();
  for( auto &sp : fParamHeaderHelper.GetSystProvider() ) {
    MF_LOG_INFO("SystToolsEventWeight") << "sp->GetToolType() = " << sp->GetToolType() << "\n"
                                  << "sp->GetFullyQualifiedName() = " << sp->GetFullyQualifiedName() << "\n"
                                  << "sp->GetInstanceName() = " << sp->GetInstanceName() << "\n"
                                  << "Printing each SystParamHeader of this ISystProviderTool..";
    // Note: typedef std::vector<SystParamHeader> SystMetaData;
    auto const& smd = sp->GetSystMetaData();

    // make a map of responsless-response params
    std::map<systtools::paramId_t, std::vector<systtools::paramId_t>> map_resp_to_respless;
    for( auto &sph : smd ){
      if(sph.isResponselessParam){
        auto it = map_resp_to_respless.find( sph.responseParamId );
        if( it != map_resp_to_respless.end() ){
          it->second.push_back( sph.systParamId );
        }
        else{
          map_resp_to_respless[sph.responseParamId] = {};
          map_resp_to_respless[sph.responseParamId].push_back( sph.systParamId );
        }
      }
    }
    if (fDebugMode) {
      for(const auto& it: map_resp_to_respless){
        const auto& sph = systtools::GetParam(smd, it.first);
        std::cout << "Found a dependent dial: " << sph.prettyName << std::endl;
        for(const auto& depdialid: it.second){
          const auto& sph_dep = systtools::GetParam(smd, depdialid);
          std::cout << "- dep dial: " << sph_dep.prettyName << std::endl;
        }
      }
    }

    for( auto &sph : smd ){

      // responsless
      if(sph.isResponselessParam){
        if (fDebugMode) {
          std::cout << "Responsless dial found: " << sph.prettyName << ", thus skipping" << std::endl;
        }
        continue;
      }

      larnusyst::evwgh::EventWeightParameterSet fParameterSet;
      EventWeightParameterSet::ReweightType rwmode = EventWeightParameterSet::kDefault;
      std::vector<double> paramVars = sph.isCorrection ? std::vector<double>(1, sph.centralParamValue) : sph.paramVariations;

      auto it = map_resp_to_respless.find( sph.systParamId );
      // if dependent dial
      if(it!=map_resp_to_respless.end()){

        for(const auto depdialid: it->second){
          const auto& sph_dep = systtools::GetParam(smd, depdialid);
          std::vector<double> paramVars_dep = sph_dep.isCorrection ? std::vector<double>(1, sph_dep.centralParamValue) : sph_dep.paramVariations;

          if(rwmode==EventWeightParameterSet::kDefault) rwmode = sph_dep.isRandomlyThrown ? EventWeightParameterSet::kMultisim : EventWeightParameterSet::kMultisigma;
          else{
            if(rwmode!=(sph_dep.isRandomlyThrown ? EventWeightParameterSet::kMultisim : EventWeightParameterSet::kMultisigma)){
              throw cet::exception{ "NuSystEventWeight" }
                << sph.prettyName << " depends on other dials, but the rwmode are different between the deps dials\n";
            }
          }

          std::vector<double> paramVariations_dep { paramVars_dep.begin(), paramVars_dep.end() };
          fParameterSet.AddParameter(sph_dep.prettyName, sph_dep.centralParamValue, std::move(paramVariations_dep));

        }

      }
      // indepedent dial
      else{

        rwmode = sph.isRandomlyThrown ? EventWeightParameterSet::kMultisim : EventWeightParameterSet::kMultisigma;

        std::vector<double> paramVariations { paramVars.begin(), paramVars.end() };

        fParameterSet.AddParameter(sph.prettyName, sph.centralParamValue, std::move(paramVariations));

      }

      if(rwmode==EventWeightParameterSet::kDefault){
        throw cet::exception{ "NuSystEventWeight" }
          << "rwmode not set for " << sph.prettyName << "\n";
      }

      if (fDebugMode) {
        std::cout << "  sph.prettyName = " << sph.prettyName << ", rwmode = " << rwmode << " is added to the header" << std::endl;
      }

      fParameterSet.SetName( sp->GetFullyQualifiedName()+"_"+sph.prettyName );
      fParameterSet.SetRWType( rwmode );
      fParameterSet.SetNUniverses( paramVars.size() );

      p->push_back( std::move(fParameterSet) );

    }
  }

  run.put(std::move(p), art::fullRun());

}

}  // namespace larnusyst::evwgh

DEFINE_ART_MODULE(larnusyst::evwgh::NuSystEventWeight)
