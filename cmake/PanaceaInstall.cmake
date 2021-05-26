
set_target_properties(panacea PROPERTIES VERSION ${panacea_VERSION} SOVERSION ${SOVERSION})

target_include_directories(panacea
  PUBLIC
    $<BUILD_INTERFACE:${panacea_SOURCE_DIR}/include/panacea>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/panacea> 
  PRIVATE
    $<BUILD_INTERFACE:${panacea_SOURCE_DIR}/src/libpanacea>
  )

install(TARGETS panacea EXPORT panaceaTargets
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )

# Install config files
install(FILES ${PROJECT_SOURCE_DIR}/cmake/panaceaConfig.cmake DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/panacea")

install(EXPORT panaceaTargets
  FILE panaceaTargets.cmake
  NAMESPACE panacea::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/panacea"
  )
