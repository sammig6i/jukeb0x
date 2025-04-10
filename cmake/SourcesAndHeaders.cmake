# Source files for the main executable (excluding main.cpp)
set(jukebox_lib_sources
    src/database/db.cpp
    src/database/sqlite.cpp
    src/utils/utils.cpp
)

# All source files for the executable target
set(jukebox_executable_sources
    src/main.cpp
    ${jukebox_lib_sources}
)

# Public header files (adjust paths if you added the jukebox/ subdirectory)
set(jukebox_headers
    include/jukebox/database/db.h
    include/jukebox/database/sqlite.h
    include/jukebox/utils/utils.h
    # Add include/jukebox/version.h here if you want it listed,
    # but it's generated, so not strictly necessary to list.
)

# Add test sources here later if needed
# set(jukebox_test_sources
#    tests/test_database.cpp
#    tests/test_utils.cpp
# )
