cmake_minimum_required(VERSION 3.11)

project("Silly Bird PSPquel")

set(CMAKE_BUILD_TYPE Debug)

add_executable(sbs-psp
    code/engine/main.cpp
    code/yyjson.c
)

include_directories(code)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)

target_include_directories(sbs-psp PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(sbs-psp PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

if(PSP)
    # Create an EBOOT.PBP file
    create_pbp_file(
        TARGET sbs-psp
        ICON_PATH "../psp-assets/icon.png"
        BACKGROUND_PATH "../psp-assets/background.png"
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
        VERSION 01.00
    )
endif()

if(WIN32)
    target_link_options(sbs-psp PRIVATE 
        "-static-libgcc" 
        "-static-libstdc++" 
        "-Wl,-Bstatic" "-lstdc++" "-lpthread" "-Wl,-Bdynamic"
    )
endif()