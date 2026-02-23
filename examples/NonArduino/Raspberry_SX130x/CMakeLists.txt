cmake_minimum_required(VERSION 3.18)

project(rpi-sx1302-gateway)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../../../RadioLib" "${CMAKE_CURRENT_BINARY_DIR}/RadioLib")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} RadioLib lgpio)

#target_compile_definitions(RadioLib PUBLIC RADIOLIB_DEBUG_BASIC RADIOLIB_DEBUG_SPI)
#target_compile_definitions(RadioLib PUBLIC RADIOLIB_DEBUG_PORT=stdout)
