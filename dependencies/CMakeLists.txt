
##################################################################
###### dynlib
##################################################################

set(DYNLIB_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dynlib" CACHE PATH "Provide the directory of the dynlib library")

if ("${DYNLIB_PROJECT_DIR}" STREQUAL "")
  message(FATAL_ERROR "The directory of the dynlib project must be specified")  
endif()

if(NOT TARGET dynlib)

  file(GLOB_RECURSE DYNLIB_SRC_FILES ${DYNLIB_PROJECT_DIR}/src/*.cpp)
  file(GLOB_RECURSE DYNLIB_HDR_FILES 
    ${DYNLIB_PROJECT_DIR}/include/*.h
    ${DYNLIB_PROJECT_DIR}/src/*.h
  )

  add_library(dynlib STATIC ${DYNLIB_HDR_FILES} ${DYNLIB_SRC_FILES})
  target_include_directories(dynlib PUBLIC "${DYNLIB_PROJECT_DIR}/include")
  target_compile_definitions(dynlib PUBLIC -DDYNLIB_STATIC_LINKING)

  if(NOT WIN32)
    set_property(TARGET dynlib PROPERTY POSITION_INDEPENDENT_CODE ON)
  endif()

  foreach(_source IN ITEMS ${DYNLIB_HDR_FILES} ${DYNLIB_SRC_FILES})
      get_filename_component(_source_path "${_source}" PATH)
      file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
      string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
      source_group("${_group_path}" FILES "${_source}")
  endforeach()

  set_target_properties(dynlib PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
  set_target_properties(dynlib PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

endif()
