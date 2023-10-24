////////////////////////////////////////////////////////////////////////
// Class: NuSystEventWeightAnalyzer
// Module Type: producer
// File: NuSystEventWeightAnalyzer_module.cc
//
// Initiated by Jaesung Kim (jae.sung.kim.3426@gmail.com)
//
////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "art/Utilities/make_tool.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/FindOne.h"
#include "canvas/Persistency/Common/FindOneP.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Common/PtrVector.h"
#include "canvas/Persistency/Common/Assns.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "larnusystematics/DataFormats/EventWeightMap.h"
#include "larnusystematics/DataFormats/EventWeightParameterSet.h"

#include "canvas/Persistency/Common/Assns.h"
#include "art/Framework/Principal/Run.h"

namespace larnusyst::evwgh {

class NuSystEventWeightAnalyzer : public art::EDAnalyzer {
public:
  explicit NuSystEventWeightAnalyzer(fhicl::ParameterSet const& p);

  NuSystEventWeightAnalyzer(NuSystEventWeightAnalyzer const &) = delete;
  NuSystEventWeightAnalyzer(NuSystEventWeightAnalyzer &&) = delete;
  NuSystEventWeightAnalyzer& operator = (NuSystEventWeightAnalyzer const&) = delete;
  NuSystEventWeightAnalyzer& operator = (NuSystEventWeightAnalyzer&&) = delete;

private:

  virtual void beginJob() override;
  virtual void beginRun(const art::Run& run) override;
  virtual void analyze (const art::Event& event) override;

private:

  std::string fGeneratorLabel, fWeightLabel;

};


NuSystEventWeightAnalyzer::NuSystEventWeightAnalyzer(fhicl::ParameterSet const& p)
  : EDAnalyzer{p}
{

  fGeneratorLabel = p.get<std::string>("GeneratorLabel");
  fWeightLabel = p.get<std::string>("WeightLabel");

}

void NuSystEventWeightAnalyzer::beginJob(){

  mf::LogInfo("NuSystEventWeightAnalyzer") << "beginJob() called" << std::endl;

}

void NuSystEventWeightAnalyzer::beginRun(const art::Run& run){

  mf::LogInfo("NuSystEventWeightAnalyzer") << "beginRun(const art::Run& run) called" << std::endl;

  art::Handle<std::vector<larnusyst::evwgh::EventWeightParameterSet>> wgt_params;
  run.getByLabel(fWeightLabel, wgt_params);

  if(!wgt_params.isValid() ) {
    std::cout << "NuSystEventWeightAnalyzer: "
              << cet::demangle_symbol(typeid(*wgt_params).name())
              << "' not found under label '" << fWeightLabel << std::endl;
    abort();
  }

  for(const larnusyst::evwgh::EventWeightParameterSet& pset: *wgt_params){
    mf::LogInfo("NuSystEventWeightAnalyzer") << pset << std::endl;
  } // end for label

}

void NuSystEventWeightAnalyzer::analyze(const art::Event& e) {

  mf::LogInfo("NuSystEventWeightAnalyzer") << "analyze(const art::Event& e) called" << std::endl;

  // Get MCTruth
  art::Handle<std::vector<simb::MCTruth>> mctruth_handle;
  e.getByLabel(fGeneratorLabel, mctruth_handle);
  if(!mctruth_handle.isValid() ) {
    mf::LogError("NuSystEventWeightAnalyzer") << cet::demangle_symbol(typeid(*mctruth_handle).name())
              << "' not found under label '" << fGeneratorLabel << std::endl;
    abort();
  }
  std::vector<art::Ptr<simb::MCTruth>> mctruths;
  if (mctruth_handle.isValid()) {
    art::fill_ptr_vector(mctruths, mctruth_handle);
  }

  art::FindManyP<larnusyst::evwgh::EventWeightMap> fmplnewm(mctruths, e, fWeightLabel);
  if(!fmplnewm.isValid()){
    mf::LogError("NuSystEventWeightAnalyzer") << "larnusyst::evwgh::EventWeightMap not found under label " << fWeightLabel << std::endl;
    abort();
  }

  for(size_t i=0; i<mctruths.size(); i++) {

    std::cout << i << "-th MCTruth" << std::endl;

    //auto const& mctruth = mctruths.at(i);

    // Find the weights associated with this particular interaction
    const std::vector<art::Ptr<larnusyst::evwgh::EventWeightMap>> wgts = fmplnewm.at(i);

    // For all the weights associated with this MCTruth
    for(const art::Ptr<larnusyst::evwgh::EventWeightMap>& wgtmap: wgts){
      for(const auto& it: *wgtmap){
        std::cout << "- ParameterSet name: " << it.first << "\nweights: ";
        for(const auto& w: it.second){
          std::cout << w << " ";
        }
        std::cout << std::endl;
      }
    } // end for wgtmap

  } // END loop mctruth

}

}  // namespace larnusyst::evwgh

DEFINE_ART_MODULE(larnusyst::evwgh::NuSystEventWeightAnalyzer)
