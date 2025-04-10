# Option to treat compiler warnings as errors
option(JUKEB0X_WARNINGS_AS_ERRORS "Treat compiler warnings as errors." OFF)

# --- Compiler & Standard Settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(JUKEB0X_ENABLE_CCACHE "Enable the usage of Ccache, in order to speed up rebuild times." ON)
if(JUKEB0X_ENABLE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        message(STATUS "Ccache found, enabling Ccache support.")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    else()
        message(STATUS "Ccache not found, disabling Ccache support.")
    endif()
endif()
