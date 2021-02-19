cmake_minimum_required(VERSION 3.9)

enable_language(CXX)

project(panacea VERSION "0.0.0")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(ENABLE_TESTING "Build and enable unit testing" OFF)

########################################################
# Compiler Flags                                       #
########################################################

include_directories(include/panacea)

########################
# Catch2 Configuration #
########################

if(ENABLE_TESTING)
  if( NOT CMAKE_BUILD_TYPE MATCHES "RELEASE")
    find_package(Catch2)
    if( NOT Catch2_FOUND )
      include(cmake/InstallCatch2.cmake)
    endif()
    include(Catch)
    include(ParseAndAddCatchTests)
  endif()
endif(ENABLE_TESTING)

########################
# Eigen Configuration  #
########################

find_package(Eigen)
if( NOT Eigen_FOUND )
  include(cmake/InstallEigen.cmake)
endif()

########################################################################
# Grab source files and set the directory where they will be installed #
########################################################################

file(GLOB SOURCES ${panacea_SOURCE_DIR}/src/libpanacea/*.cpp )
add_library(libpanacea ${SOURCES})
set_target_properties(libpanacea PROPERTIES LINKER_LANGUAGE CXX)

include(cmake/PanaceaInstall.cmake)

###############################
# Check if unit testing is on #
###############################
if(ENABLE_TESTING)
  enable_testing()
  IF(CMAKE_BUILD_TYPE MATCHES "RELEASE")
    message("Cannot build unit tests with release mode")  
  ELSE(CMAKE_BUILD_TYPE MATCHES "RELEASE")
    add_subdirectory(tst)
  ENDIF(CMAKE_BUILD_TYPE MATCHES "RELEASE")
endif(ENABLE_TESTING)

