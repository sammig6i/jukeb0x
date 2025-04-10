cmake_minimum_required(VERSION 3.13)
project(jukeb0x VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SQLiteCpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_executable(jukeb0x 
    src/main.cpp 
    src/database/db.cpp
    src/database/sqlite.cpp
    src/utils/utils.cpp
)

target_include_directories(jukeb0x PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
# Link against both libraries
target_link_libraries(jukeb0x PRIVATE 
    yaml-cpp::yaml-cpp
    SQLiteCpp
    unofficial::sqlite3::sqlite3
)
