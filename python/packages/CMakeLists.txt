
##########################
# Python3 Configuration  #
##########################

if(ENABLE_PYTHON_TESTS)
  find_package(Python3 COMPONENTS Interpreter Development)

  if(Python3_FOUND)
    include("${panacea_SOURCE_DIR}/cmake/PythonModuleCheck.cmake")

    # Python modules needed by tests
    list( APPEND PYTHON_MODULES
      pytest
      numpy
      matplotlib)

    check_python_modules("${PYTHON_MODULES}")

    file(GLOB PYTHON_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/panacea/tests/test*)

    foreach(python_test ${PYTHON_TESTS})
      get_filename_component(base_file_name "${python_test}" NAME)
      add_test (NAME "panacea_tests:python:${base_file_name}"
        COMMAND ${Python3_EXECUTABLE} -m pytest --verbose -k ${base_file_name}# Or just COMMAND pytest
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/panacea
      )
    endforeach()

  endif()
endif()
