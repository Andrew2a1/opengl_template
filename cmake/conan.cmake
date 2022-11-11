set(CONAN_TOOLCHAIN_FILENAME ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

macro(setup_conan_toolchain)
    execute_process(
        COMMAND conan install ${CMAKE_SOURCE_DIR}/conan -pr:b=default --build=missing
            -s=build_type=${CMAKE_BUILD_TYPE} -c=tools.system.package_manager:mode=install
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND_ECHO STDOUT
    )
    replace_in_file(${CONAN_TOOLCHAIN_FILENAME} "set(CMAKE_GENERATOR_PLATFORM \"x64\" CACHE STRING \"\" FORCE)" "")
    include(${CONAN_TOOLCHAIN_FILENAME})
endmacro()

function(replace_in_file FILE TEXT_TO_REPLACE TEXT)
    file(READ "${FILE}" FILE_CONTENTS)
    string(REPLACE "${TEXT_TO_REPLACE}" "${TEXT}" FILE_CONTENTS "${FILE_CONTENTS}")
    file(WRITE "${FILE}" "${FILE_CONTENTS}")
endfunction()