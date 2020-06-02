function(enable_doxygen)
  option(ENABLE_DOXYGEN "Enable doxygen doc builds of source" OFF)
  if(ENABLE_DOXYGEN)
    set(DOXYGEN_CALLER_GRAPH ON)
    set(DOXYGEN_CALL_GRAPH ON)
    set(DOXYGEN_EXTRACT_ALL ON)

    # Refer to *build-dir*/CMakeDoxyfile.in and *build-dir*/Doxyfile.doxygen-docs for a list of
    # available options.
    set(DOXYGEN_EXCLUDE_PATTERNS */tmp/* */external/* */tests/* */benchmark/*)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE Readme.md)

    find_package(Doxygen REQUIRED dot)
    doxygen_add_docs(doxygen-docs ${PROJECT_SOURCE_DIR})

  endif()
endfunction()
