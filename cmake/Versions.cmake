# Set Triplet Variables
if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x64")
    set(PROJECT_ARCH "x64")
else()
    set(PROJECT_ARCH "x32")
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(PROJECT_PLATFORM "linux")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(PROJECT_PLATFORM "osx")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(PROJECT_PLATFORM "windows")
endif()

# magic_enum variables
set(MAGIC_ENUM_VCPKG_TRIPLET "${PROJECT_ARCH}-${PROJECT_PLATFORM}")
