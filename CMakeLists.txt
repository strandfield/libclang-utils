cmake_minimum_required(VERSION 3.9)
project(libclang-utils)

set(CMAKE_CXX_STANDARD 14)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # disables warning about DLL export 
  # this is annoying when using C++ STL
  add_compile_options(/wd4251)  
endif()

set(LIBCLANGUTILS_PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})

##################################################################
###### C++17 clang (Travis CI)
##################################################################

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message("Building with clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

##################################################################
###### coverage build
##################################################################

if(ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

##################################################################
###### dependencies
##################################################################

add_subdirectory(dependencies)

##################################################################
###### libclang-utils library
##################################################################

file(GLOB_RECURSE LIBCLANGUTILS_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIBCLANGUTILS_HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/libclang-utils/*.h)

add_library(libclang-utils SHARED ${LIBCLANGUTILS_HDR_FILES} ${LIBCLANGUTILS_SRC_FILES})
target_compile_definitions(libclang-utils PRIVATE -DLIBCLANGUTILS_BUILD_LIBRARY_SHARED)
target_include_directories(libclang-utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(libclang-utils dynlib)

if(NOT WIN32)
  target_link_libraries(libclang-utils ${CMAKE_DL_LIBS})
endif()

set_target_properties(libclang-utils PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set_target_properties(libclang-utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

##################################################################
###### apps & tests
##################################################################

#add_subdirectory(apps)
add_subdirectory(tests)
