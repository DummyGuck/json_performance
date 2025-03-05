cmake_minimum_required(VERSION 3.16)

project(json_performance LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core)

add_executable(json_performance
  src/main.cpp
  glaze/include/glaze/api/qt/qvariant.hpp
  glaze/include/glaze/util/wdump.hpp
)

include(GNUInstallDirs)
install(TARGETS json_performance
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
add_subdirectory(glaze)

target_include_directories(json_performance PUBLIC ${CMAKE_SOURCE_DIR}/glaze/include/)

target_link_libraries(json_performance PRIVATE Qt5::Core glaze)



