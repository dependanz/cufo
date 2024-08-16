# file(GLOB WAVE_SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(cufo_wave cufo_wave.c ${CMAKE_CURRENT_SOURCE_DIR}/../reader/cufo_reader.c)

target_include_directories(cufo_wave PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../reader" "${CMAKE_CURRENT_SOURCE_DIR}/../cufo" "${CMAKE_CURRENT_SOURCE_DIR}/../cufo/include")