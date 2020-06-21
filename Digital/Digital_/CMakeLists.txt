cmake_minimum_required(VERSION 3.17.2)

set(LIB_NAME Digital_)

### Automatically Generate Source File List
execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Tools/GenerateSourceFilesList.bat ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_source.txt SOURCE_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_header.txt HEADER_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_all.txt ALL_FILES)

string(REPLACE "\\" "/" SOURCE_FILES    "${SOURCE_FILES}")
string(REPLACE "\\" "/" HEADER_FILES    "${HEADER_FILES}")
string(REPLACE "\\" "/" ALL_FILES       "${ALL_FILES}")

### Digital Library
add_library(${LIB_NAME} "")
add_library(LIB:${LIB_NAME} ALIAS ${LIB_NAME})
target_sources(${LIB_NAME}
    PRIVATE ${SOURCE_FILES}
    PUBLIC ${HEADER_FILES}
)
# target_include_directories(${LIB_NAME} PUBLIC ${HEADER_FILES})
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_target_properties(${LIB_NAME} PROPERTIES FOLDER Main)

### Forced Includes
find_file(DEFINES_FILE NAMES Defines.h PATHS "${CMAKE_SOURCE_DIR}/Digital_/Source/Defines")
if (DEFINES_FILE)
    if(MSVC)
        target_compile_options(${LIB_NAME} PRIVATE /FI"${CMAKE_SOURCE_DIR}/Digital_/Source/Defines/Defines.h")
    endif()
endif()

### bgfx.cmake
message("Adding SubModule: bgfx")
add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake bgfx.cmake)

### GLM - https://github.com/g-truc/glm.git
set(GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/glm)
set(GLM_LIB_NAME glm)
find_path(GLM_FOUND NAMES "glm.hpp" PATHS "${GLM_INCLUDE_DIR}/glm")
if((NOT GLM_FOUND) OR (NOT EXISTS ${GLM_INCLUDE_DIR}))
    message("Unable to find Submodule: ${GLM_LIB_NAME} or file glm.hpp")
    execute_process(COMMAND git submodule update --init -- ${GLM_INCLUDE_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
message("Found Submodule: ${GLM_LIB_NAME}")
endif()

target_include_directories(${LIB_NAME} PUBLIC "${GLM_INCLUDE_DIR}")

### Boost - 
set(BOOST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/boost)
set(BOOST_LIB_NAME Boost)
target_include_directories(${LIB_NAME} PUBLIC "${BOOST_INCLUDE_DIR}")

### Project Filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})
