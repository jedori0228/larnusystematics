art_make_library(
  LIBRARIES
    cetlib_except::cetlib_except
    cetlib::cetlib
    art::Framework_Principal
    nusimdata::SimulationBase
    nugen::EventGeneratorBase_GENIE
    fhiclcpp::fhiclcpp
)

install_headers()
install_source()
