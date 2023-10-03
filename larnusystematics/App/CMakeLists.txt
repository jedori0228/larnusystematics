cet_build_plugin(NuSystEventWeight art::module
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
