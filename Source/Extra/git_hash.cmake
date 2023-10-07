exec_program(
      "git"
      ${CMAKE_CURRENT_SOURCE_DIR}
      ARGS "rev-parse HEAD"	  
      OUTPUT_VARIABLE GIT_HASH
    )
configure_file( "Source/Extra/git_hash.template" "${CMAKE_CURRENT_BINARY_DIR}/Build/git_hash.h" )
