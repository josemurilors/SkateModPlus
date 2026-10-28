cmake_minimum_required(VERSION 3.14)
project(SkateModPlus CXX)

# O plugin-sdk exige C++20 (safetyhook) e o jogo é 32 bits: use -A Win32
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

# Plugin SDK: variável de ambiente ou -DPLUGIN_SDK_DIR=...
if(NOT PLUGIN_SDK_DIR AND DEFINED ENV{PLUGIN_SDK_DIR})
    set(PLUGIN_SDK_DIR $ENV{PLUGIN_SDK_DIR})
endif()
if(NOT PLUGIN_SDK_DIR)
    message(FATAL_ERROR "Defina PLUGIN_SDK_DIR (pasta do plugin-sdk compilado)")
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(SkateModPlus SHARED ${SOURCES})

target_include_directories(SkateModPlus PRIVATE
    src
    ${PLUGIN_SDK_DIR}
    ${PLUGIN_SDK_DIR}/shared
    ${PLUGIN_SDK_DIR}/shared/game
    ${PLUGIN_SDK_DIR}/plugin_sa
    ${PLUGIN_SDK_DIR}/plugin_sa/game_sa
    ${PLUGIN_SDK_DIR}/plugin_sa/game_sa/rw
    ${PLUGIN_SDK_DIR}/plugin_sa/game_sa/enums
    ${PLUGIN_SDK_DIR}/injector
)

target_compile_definitions(SkateModPlus PRIVATE
    GTASA PLUGIN_SGV_10US RW _CRT_SECURE_NO_WARNINGS)

target_link_directories(SkateModPlus PRIVATE ${PLUGIN_SDK_DIR}/output/lib)
target_link_libraries(SkateModPlus PRIVATE Plugin d3d9 user32 kernel32 gdi32 shell32 advapi32)

set_target_properties(SkateModPlus PROPERTIES
    OUTPUT_NAME "SkateMod+"
    SUFFIX ".asi"
)

# Teste do leitor de ini (não precisa do jogo nem do SDK)
enable_testing()
add_executable(config_test tests/config_test.cpp src/core/Config.cpp)
target_include_directories(config_test PRIVATE src)
add_test(NAME config_test COMMAND config_test)
