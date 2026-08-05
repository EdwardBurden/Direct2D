cmake_minimum_required(VERSION 3.10)
project(2DWINDOWS VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE 2DWINDOWS "src/*.cpp")
add_executable(${PROJECT_NAME} WIN32 ${2DWINDOWS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includes)


if(MSVC)
    target_link_options(2DWINDOWS PRIVATE
        "/MANIFEST:EMBED"
        "/MANIFESTINPUT:${CMAKE_CURRENT_SOURCE_DIR}/partial.manifest"
    )
endif()