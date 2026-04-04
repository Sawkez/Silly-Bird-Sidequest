cmake_minimum_required(VERSION 3.11)

project("Silly Bird Sidequest")

set(CMAKE_BUILD_TYPE Debug)
set(YYJSON_DIR "code/3rdparty/yyjson/src")
set(LVGL_DIR "code/3rdparty/lvgl")

set(LV_BUILD_CONF_PATH "${CMAKE_SOURCE_DIR}/code/3rdparty/lv_conf.h" CACHE PATH "Path to custom lv_conf.h" FORCE)
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(code)
add_subdirectory(code/3rdparty/lvgl)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)

add_executable(sbsidequest
    code/engine/main.cpp
    ${YYJSON_DIR}/yyjson.c
)

target_include_directories(sbsidequest PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${YYJSON_DIR}
)

target_link_libraries(sbsidequest PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    lvgl
)

if(PSP)
    # Create an EBOOT.PBP file
    create_pbp_file(
        TARGET sbsidequest
        ICON_PATH "${CMAKE_SOURCE_DIR}/psp-assets/icon.png"
        BACKGROUND_PATH "${CMAKE_SOURCE_DIR}/psp-assets/background.png"
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
        VERSION 01.00
    )
endif()

if(WIN32)
    target_link_options(sbsidequest PRIVATE 
        "-static-libgcc" 
        "-static-libstdc++" 
        "-Wl,-Bstatic" "-lstdc++" "-lpthread" "-Wl,-Bdynamic"
    )
endif()