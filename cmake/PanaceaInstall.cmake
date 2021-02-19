target_include_directories(libpanacea PUBLIC
  $<BUILD_INTERFACE:${panacea_SOURCE_DIR}/src/libpanacea>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/panacea>
  )

message("Instal include dir ${CMAKE_INSTALL_INCLUDEDIR}/panacea")

install(TARGETS libpanacea EXPORT panaceaTargets
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )

# Install config files
install(FILES ${PROJECT_SOURCE_DIR}/cmake/PanaceaConfig.cmake DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/panacea")

install(EXPORT panaceaTargets
  FILE panaceaTargets.cmake
  NAMESPACE panacea::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/panacea"
  )
