cmake_minimum_required(VERSION 3.13)
project(jukeb0x VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SQLiteCpp CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_subdirectory(src)

