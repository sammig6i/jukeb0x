cmake_minimum_required(VERSION 3.13)
project(jukeb0x VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(StandardSettings)

include(CompilerWarnings)
include(SourcesAndHeaders)

find_package(SQLiteCpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)
configure_file(
    cmake/version.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/include/jukebox/version.h"
)

add_executable(jukeb0x ${jukebox_executable_sources})


target_include_directories(jukeb0x PRIVATE
    "${CMAKE_SOURCE_DIR}/include"                 # For project headers
    "${CMAKE_CURRENT_BINARY_DIR}/include"         # For generated version.h
)

target_link_libraries(jukeb0x PRIVATE
    yaml-cpp::yaml-cpp
    SQLiteCpp
    unofficial::sqlite3::sqlite3
)

set_project_warnings(jukeb0x) # Call *after* add_executable

# --- Testing Setup (Placeholder) ---
# If you add tests later, you might use add_subdirectory(tests) here
# enable_testing()
# add_subdirectory(tests)

# --- Optional: Add Install Rules (Example) ---
# include(GNUInstallDirs)
# install(TARGETS jukeb0x DESTINATION ${CMAKE_INSTALL_BINDIR})
# install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/jukebox" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/jukebox/version.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jukebox)
