art_dictionary(DICTIONARY_LIBRARIES larnusystematics_DataFormats nusimdata::SimulationBase)

cet_make_library(SOURCE
  EventWeightMap.cxx
  EventWeightParameterSet.cxx
  LIBRARIES
  canvas::canvas
  PUBLIC
  nusimdata::SimulationBase
)

install_headers()
install_source()

