cmake_minimum_required(VERSION 3.18.3)

enable_language(CXX)

project(panacea VERSION "0.0.1")

set(SOVERSION 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(ENABLE_TESTING "Build and enable unit testing" ON)
option(ENABLE_MEMORY_TESTING "Build and enable unit testing" ${ENABLE_TESTING})
option(ENABLE_PYTHON_TESTS "Add python tests to the testing infrastructure" ${ENABLE_TESTING})
########################################################
# Includes                                             #
########################################################
# This include must be placed before using the variables:
# CMAKE_INSTALL_INCLUDEDIR
# CMAKE_INSTALL_LIBDIR
include(GNUInstallDirs)

# Public includes
include_directories(${panacea_SOURCE_DIR}/include)

# Private includes
include_directories(${panacea_SOURCE_DIR}/src/libpanacea)

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

find_package(Eigen3)
if( NOT Eigen3_FOUND )
  include(cmake/InstallEigen.cmake)
endif()

########################################################################
# Grab source files                                                    #
########################################################################

file(GLOB SOURCES ${panacea_SOURCE_DIR}/src/libpanacea/*.cpp
  ${panacea_SOURCE_DIR}/src/libpanacea/*
  ${panacea_SOURCE_DIR}/src/libpanacea/attributes/*
  ${panacea_SOURCE_DIR}/src/libpanacea/attributes/covariance/*
  ${panacea_SOURCE_DIR}/src/libpanacea/attribute_manipulators/*
  ${panacea_SOURCE_DIR}/src/libpanacea/descriptors/*
  ${panacea_SOURCE_DIR}/src/libpanacea/distribution/*
  ${panacea_SOURCE_DIR}/src/libpanacea/distribution/distributions/*
  ${panacea_SOURCE_DIR}/src/libpanacea/distribution/distributions/kernel_distribution/*
  ${panacea_SOURCE_DIR}/src/libpanacea/distribution/distribution_settings/*
  ${panacea_SOURCE_DIR}/src/libpanacea/entropy/*
  ${panacea_SOURCE_DIR}/src/libpanacea/entropy/entropy_settings/*
  ${panacea_SOURCE_DIR}/src/libpanacea/entropy/entropy_terms/*
  ${panacea_SOURCE_DIR}/src/libpanacea/entropy/entropy_terms/entropy_decorators/*
  ${panacea_SOURCE_DIR}/src/libpanacea/io/*
  ${panacea_SOURCE_DIR}/src/libpanacea/kernels/*
  ${panacea_SOURCE_DIR}/src/libpanacea/matrix/*
  ${panacea_SOURCE_DIR}/src/libpanacea/normalization_methods/*
  ${panacea_SOURCE_DIR}/src/libpanacea/primitives/*
  ${panacea_SOURCE_DIR}/src/libpanacea/tasks/*
  ${panacea_SOURCE_DIR}/src/libpanacea/vector/*
  )

####################################################
# Setup the directory where they will be installed #
####################################################

add_library(panacea ${SOURCES})
set_target_properties(panacea PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(panacea Eigen3::Eigen)

###############################
# Add subdirectories
###############################
add_subdirectory(include/panacea)

###############################
# Setup installation
###############################
include(cmake/PanaceaInstall.cmake)

###############################
# Check if unit testing is on #
###############################

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tst)
  add_subdirectory(python/packages)
endif(ENABLE_TESTING)

