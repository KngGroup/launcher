cmake_minimum_required (VERSION 3.0)
project (Launcher)

set (Launcher_PACKAGE Contacts)
set (Launcher_VERSION_MAJOR 0)
set (Launcher_VERSION_MINOR 0)
set (Launcher_VERSION_PATCH 1)
set (Launcher_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

configure_file (
  "${Launcher_INCLUDE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )


include_directories(
  "${PROJECT_BINARY_DIR}"
  "${Launcher_INCLUDE_DIR}"
)


find_library(LIBWEBSOCKETS NAMES websockets)

add_executable(launcher main.c)
target_link_libraries(
  launcher
  "${LIBWEBSOCKETS}"
)
