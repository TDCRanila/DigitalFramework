cmake_minimum_required(VERSION 3.17.2)

project(libmap LANGUAGES CXX)

set(LIBMAP_CMAKE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libmap)

set(LIBMAP_HEADER_FILES 
    ${LIBMAP_CMAKE_INCLUDE_DIR}/brush.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/entity.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/entity_geometry.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/face.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/geo_generator.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/libmap.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/libmap_math.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/map_data.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/map_parser.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/matrix.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/platform.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/surface_gatherer.h
    ${LIBMAP_CMAKE_INCLUDE_DIR}/vector.h
)

set(LIBMAP_SOURCE_FILES
    ${LIBMAP_CMAKE_INCLUDE_DIR}/entity.cpp
    ${LIBMAP_CMAKE_INCLUDE_DIR}/geo_generator.cpp
    ${LIBMAP_CMAKE_INCLUDE_DIR}/map_data.cpp
    ${LIBMAP_CMAKE_INCLUDE_DIR}/map_parser.cpp
    ${LIBMAP_CMAKE_INCLUDE_DIR}/matrix.cpp
    ${LIBMAP_CMAKE_INCLUDE_DIR}/surface_gatherer.cpp
    ${LIBMAP_CMAKE_INCLUDE_DIR}/vector.cpp
)

add_library(libmap
    ${LIBMAP_HEADER_FILES}
    ${LIBMAP_SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${LIBMAP_CMAKE_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER libmap)
