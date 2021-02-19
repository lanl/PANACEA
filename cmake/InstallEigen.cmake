# If Eigen is not found, instead use the git submodule
if (NOT EXISTS ${panacea_SOURCE_DIR}/external/eigen/CMakeLists.txt)
  # Clone the submodule
  execute_process(COMMAND git submodule update --init --force -- external/eigen WORKING_DIRECTORY ${panacea_SOURCE_DIR})
endif()

list(APPEND CMAKE_MODULE_PATH "${panacea_SOURCE_DIR}/external/eigen/cmake")
add_subdirectory(${panacea_SOURCE_DIR}/external/eigen)

