
file(GLOB PYTHON_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/panacea/tests/test*)

foreach(python_test ${PYTHON_TESTS})
  get_filename_component(base_file_name "${python_test}" NAME)
  add_test (NAME "panacea_tests:python:${base_file_name}"
    COMMAND ${Python3_EXECUTABLE} -m pytest --verbose -k ${base_file_name}# Or just COMMAND pytest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/panacea
  )
endforeach()

