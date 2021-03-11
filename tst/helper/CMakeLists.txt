
list( APPEND HELPER_SOURCES
  helper.cpp
  )

add_library(libhelper ${HELPER_SOURCES})
set_target_properties(libhelper PROPERTIES LINKER_LANGUAGE CXX)
