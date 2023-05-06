
enable_testing()

if(NOT DEFINED CACHE{BUILD_LIBCLANGUTILS_TESTS})
  set(BUILD_LIBCLANGUTILS_TESTS ON CACHE BOOL "whether to build libclang-utils test")
endif()

if(BUILD_LIBCLANGUTILS_TESTS)

  if(NOT DEFINED LIBCLANGUTILS_CATCH2_SINGLE_HEADER_FILE OR NOT LIBCLANGUTILS_CATCH2_SINGLE_HEADER_FILE)

    message("Downloading Catch2")
    file(DOWNLOAD https://github.com/catchorg/Catch2/releases/download/v2.13.10/catch.hpp "${CMAKE_CURRENT_LIST_DIR}/catch.hpp" SHOW_PROGRESS)
  
    set(LIBCLANGUTILS_CATCH2_SINGLE_HEADER_FILE "${CMAKE_CURRENT_LIST_DIR}/catch.hpp" CACHE STRING "Path of catch.hpp")

  endif()

  add_executable(TEST_libclangutils "tests.cpp" ${LIBCLANGUTILS_CATCH2_SINGLE_HEADER_FILE})
  target_link_libraries(TEST_libclangutils libclang-utils)

  set_target_properties(TEST_libclangutils PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(TEST_libclangutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

  add_test(NAME TEST_libclangutils COMMAND TEST_libclangutils)

endif()
