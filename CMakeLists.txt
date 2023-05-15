cmake_minimum_required(VERSION 3.21)
project(fractal LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(fractal src/main.cpp)
target_link_libraries(fractal PRIVATE sfml-graphics)
target_compile_features(fractal PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET fractal POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:fractal> $<TARGET_FILE_DIR:fractal> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS fractal)
