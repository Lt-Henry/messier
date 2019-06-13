cmake_minimum_required(VERSION 3.0)

project(messier)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PLY_CORE REQUIRED ply-splash-core)
pkg_check_modules(PLY_GRAPHICS REQUIRED ply-splash-graphics)
pkg_check_modules(SYSTEMD REQUIRED libsystemd)

add_library(messier SHARED plugin.c)
target_include_directories(messier PUBLIC ${PLY_CORE_INCLUDE_DIRS} ${PLY_GRAPHICS_INCLUDE_DIRS} ${SYSTEMD_INCLUDE_DIRS})
target_link_libraries(messier ${PLY_CORE_LIBRARIES} ${PLY_GRAPHICS_LIBRARIES} ${SYSTEMD_LIBRARIES})

