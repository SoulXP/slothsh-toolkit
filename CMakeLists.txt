cmake_minimum_required(VERSION 3.15.0)
project(slothsh-toolkit VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "Project source path: ${CMAKE_SOURCE_DIR}")

# Handle enviroment variables
if (NOT BUILD_CONFIG STREQUAL "debug" AND NOT BUILD_CONFIG STREQUAL "release")
    set(BUILD_CONFIG "release")
endif()

if (NOT WARNING_CONFIG STREQUAL "all" AND NOT WARNING_CONFIG STREQUAL "normal")
    set(WARNING_CONFIG "normal")
endif()

# Global compiler configuration
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    set(CMAKE_CXX_STANDARD 23)
else()
    set(CMAKE_CXX_STANDARD 20)
endif()

# MSVC compiler parameters
if (MSVC)
    if (${BUILD_CONFIG} STREQUAL "debug")
        set(LINK_RUNTIME_LIB "/MTd")
    else()
        set(LINK_RUNTIME_LIB "/MT")
    endif()

    if (${WARNING_CONFIG} STREQUAL "verbose")
        set(COMPILE_WARNINGS "/W4")
    else()
        set(COMPILE_WARNINGS "/W2")
    endif()

    set(LINK_RUNTIME_EXCEPTIONS "/EHsc")
    add_compile_options(${LINK_RUNTIME_LIB} ${COMPILE_WARNINGS} ${LINK_RUNTIME_EXCEPTIONS})

    message(STATUS "Setting compiler warnings to ${COMPILE_WARNINGS}")
    message(STATUS "Linking run time library to ${LINK_RUNTIME_LIB}")
    message(STATUS "Setting exception handling model to ${LINK_RUNTIME_LIB}")

# Unix compiler parameters
else()
    if (${WARNING_CONFIG} STREQUAL "all")
        set(COMPILE_WARNINGS "-Wall" "-Wpedantic" "-Werror" "-Wextra")
    else()
        set(COMPILE_WARNINGS "-Wall" "-Werror")
    endif()

    add_compile_options(${COMPILE_WARNINGS})
    message(STATUS "Setting compiler warnings to ${COMPILE_WARNINGS}")
endif()

# Build Third-Party Dependencies via vcpkg
find_package(magic_enum CONFIG REQUIRED)

# Include local and third-party headers
include_directories(
    include
)

# Build Targets
set(SRC_SLOTHSHTOOLKIT ./main.cpp)
add_executable(helloworld ${SRC_SLOTHSHTOOLKIT})
target_link_libraries(helloworld PRIVATE fmt::fmt magic_enum::magic_enum)

# Create Compile Commands Symlink for clangd
message(STATUS "Creating compile_commands.json symlink to ${CMAKE_BINARY_DIR}/compile_commands.json")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Configure tests, benchmarks, and examples
# enable_testing()
# add_subdirectory(tests)
# add_subdirectory(benchmarks)
# add_subdirectory(examples)
