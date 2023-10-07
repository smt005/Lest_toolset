exec_program(
      "git"
      ${CMAKE_CURRENT_SOURCE_DIR}
	  ARGS "rev-parse --abbrev-ref HEAD"
	  OUTPUT_VARIABLE GIT_NAME_BRANCH
    )
configure_file( "Source/Extra/git_name_branch.template" "${CMAKE_CURRENT_BINARY_DIR}/Build/git_name_branch.h" )
