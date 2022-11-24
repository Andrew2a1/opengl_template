function(setup_dependencies)
    find_package(FreeGLUT REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glm REQUIRED)
    find_package(stb REQUIRED)
endfunction()

function(setup_app APP_TARGET_NAME)
    target_include_directories(${APP_TARGET_NAME}
    PUBLIC
        include/
    )

    target_link_libraries(${APP_TARGET_NAME}
        FreeGLUT::freeglut_static
        GLEW::GLEW
        glm::glm
        stb::stb
    )

    set_target_properties(${APP_TARGET_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED TRUE
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    if (MSVC)
        target_compile_options(${APP_TARGET_NAME} PRIVATE /W4 $<IF:$<CONFIG:Debug>,/Zi,/O2>)
    else()
        target_compile_options(${APP_TARGET_NAME} PRIVATE -Wall -Wextra -pedantic $<IF:$<CONFIG:Debug>,-g,-O3>)
    endif()
endfunction()