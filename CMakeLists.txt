cmake_minimum_required(VERSION 3.5)
project(cufo
    LANGUAGES C
)

add_subdirectory(reader)
add_subdirectory(json)
add_subdirectory(wave)

file(GLOB CUFO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/cufo/*.c ${CMAKE_CURRENT_SOURCE_DIR}/cufo/src/*.c)
add_executable(cufo main.c ${CUFO_SRC})

target_link_libraries(cufo PUBLIC cufo_reader cufo_json cufo_wave)
target_include_directories(cufo PUBLIC cufo cufo/include reader json wave)

# cufo_lib
file(GLOB CUFO_LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/cufo/*.c ${CMAKE_CURRENT_SOURCE_DIR}/cufo/src/*.c)
add_library(cufo_lib ${CUFO_LIB_SRC})

target_link_libraries(cufo_lib PUBLIC 
    cufo_reader 
    cufo_json
    cufo_wave
)
target_include_directories(cufo_lib PUBLIC 
    cufo cufo/include
    ${CMAKE_CURRENT_SOURCE_DIR}/reader 
    ${CMAKE_CURRENT_SOURCE_DIR}/json
    ${CMAKE_CURRENT_SOURCE_DIR}/wave
)

# message(CMAKE_COMMAND="${CMAKE_COMMAND}")
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/test_files)
    make_directory(${CMAKE_CURRENT_BINARY_DIR}/test_files)
endif()

add_custom_target(
    copy_my_folder
    ALL # always build the target (essentially equivalent to "all:" in makefiles)
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/test_files
        ${CMAKE_CURRENT_BINARY_DIR}/test_files
)