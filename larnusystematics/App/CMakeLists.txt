cet_build_plugin(NuSystEventWeightProducer art::module
  LIBRARIES
  larnusystematics_DataFormats
  systtools::interface
  systtools::interpreters
  systtools::utility
  nusyst::interface
  nusyst::systproviders
  nusyst::responsecalculators
  nugen::EventGeneratorBase_GENIE
  nusimdata::SimulationBase
  ${GENIE_LIB_LIST}
)
cet_build_plugin(NuSystEventWeightAnalyzer art::module
  LIBRARIES
  larnusystematics_DataFormats
  systtools::interface
  systtools::interpreters
  systtools::utility
  nusyst::interface
  nusyst::systproviders
  nusyst::responsecalculators
  nugen::EventGeneratorBase_GENIE
  nusimdata::SimulationBase
  ${GENIE_LIB_LIST}
)

install_headers()
install_source()
