# Declare Project
cmake_minimum_required(VERSION 3.15.0)
project(slothshtoolkit VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "project: ${PROJECT_NAME}")
message(STATUS "cmake project: ${CMAKE_PROJECT_NAME}")

# Include Version Info
include(cmake/Versions.cmake)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(IS_TOPLEVEL_PROJECT TRUE)
else()
    set(IS_TOPLEVEL_PROJECT FALSE)
endif()

option(SLOTHSH_TOOLKIT_OPT_BUILD_BENCHMARKS "Build slothsh-toolkit examples" ${IS_TOPLEVEL_PROJECT})
option(SLOTHSH_TOOLKIT_OPT_BUILD_EXAMPLES "Build slothsh-toolkit examples" ${IS_TOPLEVEL_PROJECT})
option(SLOTHSH_TOOLKIT_OPT_BUILD_TESTS "Build and perform slothsh-toolkit tests" ${IS_TOPLEVEL_PROJECT})
option(SLOTHSH_TOOLKIT_OPT_INSTALLL "Generate and install slothsh-toolkit target" ${IS_TOPLEVEL_PROJECT})

# if(SLOTHSH_TOOLKIT_OPT_BUILD_EXAMPLES)
#     add_subdirectory(examples)
# endif()
#
# if(SLOTHSH_TOOLKIT_OPT_BUILD_TESTS)
#     enable_testing()
#     add_subdirectory(tests)
# endif()
#
# if(SLOTHSH_TOOLKIT_OPT_BUILD_BENCHMARKS)
#     add_subdirectory(benchmarks)
# endif()

# Handle enviroment variables
if (NOT BUILD_CONFIG STREQUAL "debug" AND NOT BUILD_CONFIG STREQUAL "release")
    set(BUILD_CONFIG "release")
endif()

# Global compiler configuration
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    set(CMAKE_CXX_STANDARD 23)
    set(COMPILE_FEATURES cxx_std_23)
else()
    set(CMAKE_CXX_STANDARD 20)
    set(COMPILE_FEATURES cxx_std_20)
endif()

# MSVC compiler parameters
if (MSVC)
    if (${BUILD_CONFIG} STREQUAL "debug")
        set(LINK_RUNTIME_LIB "/MTd")
    else()
        set(LINK_RUNTIME_LIB "/MT")
    endif()

    set(COMPILE_WARNINGS "/W4")
    set(LINK_RUNTIME_EXCEPTIONS "/EHsc")
    add_compile_options(${LINK_RUNTIME_LIB} ${COMPILE_WARNINGS} ${LINK_RUNTIME_EXCEPTIONS})

    message(STATUS "Setting compiler warnings to ${COMPILE_WARNINGS}")
    message(STATUS "Linking run time library to ${LINK_RUNTIME_LIB}")
    message(STATUS "Setting exception handling model to ${LINK_RUNTIME_LIB}")

# Unix compiler parameters
else()
    set(COMPILE_WARNINGS "-Wall" "-Wpedantic" "-Werror" "-Wextra")
    add_compile_options(${COMPILE_WARNINGS})
    if (SLOTHSH_TOOLKIT_DEV STREQUAL "on")
        set(COMPILE_DEFINITIONS "-DSLOTHSH_TOOLKIT_DEV")
    else()
        set(COMPILE_DEFINITIONS)
    endif()
    add_compile_definitions(${COMPILE_DEFINITIONS})
    message(STATUS "Setting compiler warnings to ${COMPILE_WARNINGS}")
    message(STATUS "Setting compiler definitions to ${COMPILE_DEFINITIONS}")
endif()

# Build Third-Party Dependencies via vcpkg
find_package(magic_enum CONFIG REQUIRED)

# Build Targets
add_library(slothshtoolkit INTERFACE)
target_include_directories(slothshtoolkit INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(slothshtoolkit INTERFACE magic_enum::magic_enum)

# Create Compile Flags Symlink for clangd
if (${IS_TOPLEVEL_PROJECT})
    include(cmake/Functions.cmake)
    message(STATUS "Creating compile_commands.json symlink to ${CMAKE_BINARY_DIR}/compile_commands.json")
    write_clangd_compile_flags("-std=gnu++${CMAKE_CXX_STANDARD}${CMAKE_CXX_FLAGS_DEBUG}${COMPILE_WARNINGS}${COMPILE_DEFINITIONS}")
    create_clangd_compile_flags_symlink()
endif()

# Install Header Files
if(SLOTHSH_TOOLKIT_OPT_INSTALLL)
    # GNU Install Directories
    include(GNUInstallDirs)

    # Generate Cmake Targets
    install(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # Generate Package Version
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                     VERSION ${PROJECT_VERSION}
                                     COMPATIBILITY SameMajorVersion)
    
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})
    
    # Install Cmake Config & Version Info
    install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})

    export(EXPORT ${PROJECT_NAME}Config
           NAMESPACE ${PROJECT_NAME}::)

    # Install Project Header Files
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION .)

    # Install Project Local magic_enum Headers
    # set(MAGIC_ENUM_INSTALL_DIR ./include/slsh/magic_enum)
    # set(MAGIC_ENUM_LICENSE ${CMAKE_BINARY_DIR}/vcpkg_installed/${MAGIC_ENUM_VCPKG_TRIPLET}/share/magic-enum/copyright)
    # set(MAGIC_ENUM_FILES
    #     ${CMAKE_BINARY_DIR}/vcpkg_installed/${MAGIC_ENUM_VCPKG_TRIPLET}/include/magic_enum.hpp
    #     ${CMAKE_BINARY_DIR}/vcpkg_installed/${MAGIC_ENUM_VCPKG_TRIPLET}/include/magic_enum_format.hpp
    #     ${CMAKE_BINARY_DIR}/vcpkg_installed/${MAGIC_ENUM_VCPKG_TRIPLET}/include/magic_enum_fuse.hpp) 
    #
    # install(FILES ${MAGIC_ENUM_LICENSE}
    #         RENAME "LICENSE"
    #         DESTINATION ${MAGIC_ENUM_INSTALL_DIR})
    # install(FILES ${MAGIC_ENUM_FILES}
    #         DESTINATION ${MAGIC_ENUM_INSTALL_DIR})
endif()
