cmake_minimum_required(VERSION 3.21)

include(cmake/prelude.cmake)

project(
    glaze
    VERSION 4.3.0
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

add_library(glaze INTERFACE)
add_library(glaze::glaze ALIAS glaze)


if (MSVC)
  string(REGEX MATCH "\/cl(.exe)?$" matched_cl ${CMAKE_CXX_COMPILER})
  if (matched_cl)
    # for a C++ standards compliant preprocessor, not needed for clang-cl
    target_compile_options(glaze INTERFACE "/Zc:preprocessor" /permissive- /Zc:lambda)
    
    if(PROJECT_IS_TOP_LEVEL)
      target_compile_options(glaze INTERFACE
        $<$<CONFIG:Release>:/GL>
        $<$<CONFIG:MinSizeRel>:/GL>)
      target_link_options(glaze INTERFACE
        $<$<CONFIG:Release>:/LTCG /INCREMENTAL:NO>
        $<$<CONFIG:MinSizeRel>:/LTCG /INCREMENTAL:NO>)
    endif()
   endif()
else()
  target_compile_options(glaze INTERFACE "-Wno-missing-braces")
endif()

set_property(TARGET glaze PROPERTY EXPORT_NAME glaze)

target_compile_features(glaze INTERFACE cxx_std_23)
target_include_directories(
    glaze ${warning_guard}
    INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)
