file(GLOB JSON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../reader ${CMAKE_CURRENT_BINARY_DIR}/../reader)

add_library(cufo_json cufo_json.c ${JSON_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/../reader/cufo_reader.c)

target_include_directories(cufo_json PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/../reader")