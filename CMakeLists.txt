cmake_minimum_required(VERSION 3.16)

project("Silly Bird Sidequest")

set(CMAKE_BUILD_TYPE Debug)

set(LV_BUILD_CONF_PATH "${CMAKE_SOURCE_DIR}/code/3rdparty/lv_conf.h" CACHE PATH "Path to custom lv_conf.h" FORCE)
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SDL_X11_XSCRNSAVER OFF CACHE BOOL "" FORCE)
set(SDL3_TESTS OFF CACHE BOOL "" FORCE)
set(SDL3_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL3_IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL3_IMAGE_TESTS OFF CACHE BOOL "" FORCE)

set(SDL3_IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL3IMAGE_PNG ON CACHE BOOL "" FORCE)

set(SDL3IMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_BMP OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_GIF OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_JPG OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_JXL OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_LBM OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_PCX OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_PNM OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_QOI OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_SVG OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_TGA OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_TIF OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_WEBP OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_XCF OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_XPM OFF CACHE BOOL "" FORCE)
set(SDL3IMAGE_XV OFF CACHE BOOL "" FORCE)

include_directories(${CMAKE_SOURCE_DIR}/code)
add_subdirectory(${CMAKE_SOURCE_DIR}/code/3rdparty/lvgl)
add_subdirectory(${CMAKE_SOURCE_DIR}/code/3rdparty/yyjson)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/code/3rdparty/SDL)
add_subdirectory(${CMAKE_SOURCE_DIR}/code/3rdparty/SDL_image)

add_executable(sbsidequest code/engine/main.cpp)

target_link_libraries(sbsidequest PRIVATE
    yyjson
    lvgl
    SDL3::SDL3
    SDL3_image::SDL3_image
)

if(WIN32)
    target_link_options(sbsidequest PRIVATE 
        "-static-libgcc" 
        "-static-libstdc++" 
        "-Wl,-Bstatic" "-lstdc++" "-lpthread" "-Wl,-Bdynamic"
    )
endif()