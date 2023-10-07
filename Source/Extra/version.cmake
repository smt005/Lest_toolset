exec_program(
      "git"
      ${CMAKE_CURRENT_SOURCE_DIR}
      ARGS "rev-parse HEAD"
      OUTPUT_VARIABLE GIT_HASH
    )
configure_file( "Source/Extra/version.h.template" "${CMAKE_CURRENT_BINARY_DIR}/Build/version.h" )
