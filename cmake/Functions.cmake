function(write_clangd_compile_flags all_flags)
    string(REPLACE "-" " -" COMPILE_FLAGS "${all_flags}")
    string(REPLACE ";" "" COMPILE_FLAGS ${COMPILE_FLAGS})
    string(REGEX REPLACE "^ +| $" "" COMPILE_FLAGS ${COMPILE_FLAGS})
    string(APPEND COMPILE_FLAGS " -I${CMAKE_BINARY_DIR}/vcpkg_installed/${MAGIC_ENUM_VCPKG_TRIPLET}/include")
    string(REPLACE " " "\n" COMPILE_FLAGS ${COMPILE_FLAGS})
    file(WRITE ${CMAKE_BINARY_DIR}/compile_flags.txt ${COMPILE_FLAGS})
endfunction()

function(create_clangd_compile_flags_symlink)
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/compile_flags.txt ${CMAKE_SOURCE_DIR}/compile_flags.txt
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endfunction()
