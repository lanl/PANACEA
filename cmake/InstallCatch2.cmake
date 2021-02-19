# If Catch2 is not found, instead use the git submodule
if (NOT EXISTS ${panacea_SOURCE_DIR}/external/Catch2/single_include)
  # Unable to find the header files for Catch2 or they don't exist
  # Clone the submodule
  execute_process(COMMAND git submodule update --init --force -- external/Catch2 WORKING_DIRECTORY ${panacea_SOURCE_DIR})
  execute_process(COMMAND git checkout v2.13.2 WORKING_DIRECTORY ${panacea_SOURCE_DIR}/external/Catch2)
endif()

list(APPEND CMAKE_MODULE_PATH "${panacea_SOURCE_DIR}/external/Catch2/contrib")
add_subdirectory(${panacea_SOURCE_DIR}/external/Catch2)

