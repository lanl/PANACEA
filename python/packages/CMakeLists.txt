
add_test (NAME python-tests
  COMMAND ${Python3_EXECUTABLE} -m pytest --verbose# Or just COMMAND pytest
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/panacea
)
