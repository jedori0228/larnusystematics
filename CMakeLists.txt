# ======================================================================
#  larnusystematics main build file
#
# ======================================================================


# use cmake 3.3 or later
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

find_package(cetmodules REQUIRED)
project(larnusystematics VERSION 09.00.00 LANGUAGES CXX)

if(NOT DEFINED USEART) #by default build in ART mode
  SET(USEART TRUE)
endif()

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic )

cet_report_compiler_flags(REPORT_THRESHOLD VERBOSE)

find_package(GSL REQUIRED)
find_package(art REQUIRED EXPORT)
find_package(art_root_io REQUIRED EXPORT)
find_package(nusystematics REQUIRED EXPORT)
find_package(systematicstools REQUIRED EXPORT)
find_package(nusimdata REQUIRED EXPORT)
find_package(nugen REQUIRED EXPORT)
find_package(log4cpp REQUIRED EXPORT)
find_package(ifdh_art REQUIRED EXPORT)
find_package(LHAPDF REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Pythia6 REQUIRED)
find_package(GENIE REQUIRED EXPORT)

# macros for dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)


# ADD SOURCE CODE SUBDIRECTORIES HERE
add_subdirectory(larnusystematics)

# tests
add_subdirectory(test)

# ups - table and config files
# packaging utility
cet_cmake_config()

