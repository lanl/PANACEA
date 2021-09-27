
function(check_python_modules python_modules)
  foreach( python_module ${python_modules})
    if( "${python_module}" STREQUAL "pytest")
      execute_process(COMMAND ${Python3_EXECUTABLE} -m ${python_module} --version
        OUTPUT_VARIABLE MODULE_output
        ERROR_VARIABLE  MODULE_error
        RESULT_VARIABLE MODULE_result)
      if( NOT ${MODULE_result} EQUAL 0)
        message(SEND_ERROR "Pytest package not available, if you do not need \
to use the python scripts you can turn the python tests off with the \
ENABLE_PYTHON_TESTS flag. ${MODULE_error}")
      endif()
    else()
      execute_process(COMMAND ${Python3_EXECUTABLE} -c "import ${python_module}"
        OUTPUT_VARIABLE MODULE_output
        ERROR_VARIABLE  MODULE_error
        RESULT_VARIABLE MODULE_result)
      if( NOT ${MODULE_result} EQUAL 0)
        message(SEND_ERROR "${python_module} module not available: ${MODULE_error}")
        message(SEND_ERROR "{python_module} module not available, if you do \
not need to use the python scripts you can turn the python tests off with the \
ENABLE_PYTHON_TESTS flag. ${MODULE_error}")
      endif()
    endif()
  endforeach()
endfunction()
