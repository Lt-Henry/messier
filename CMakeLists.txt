cmake_minimum_required(VERSION 3.0)

project(messier)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PLY_CORE REQUIRED ply-splash-core)

add_library(plugin SHARED plugin.c)
target_include_directories(plugin PUBLIC ${PLY_CORE_INCLUDE_DIRS})
target_link_libraries(plugin ${PLY_CORE_LIBRARIES})

