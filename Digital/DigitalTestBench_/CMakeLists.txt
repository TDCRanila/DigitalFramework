cmake_minimum_required(VERSION 3.17.2)

set(EXE_NAME DigitalTestBench_)
project(${EXE_NAME} VERSION 1.0.0)

### Automatically Generate Source File List
execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Tools/GenerateSourceFilesList.bat ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_source.txt SOURCE_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_header.txt HEADER_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_all.txt ALL_FILES)
string(REPLACE "\\" "/" SOURCE_FILES    "${SOURCE_FILES}")
string(REPLACE "\\" "/" HEADER_FILES    "${HEADER_FILES}")
string(REPLACE "\\" "/" ALL_FILES       "${ALL_FILES}")

### Executable Definition
add_executable(${EXE_NAME})
target_sources(${EXE_NAME}
    PRIVATE 
    ${ALL_FILES}
)

target_include_directories(${EXE_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(${EXE_NAME} PRIVATE LIB:Digital_)
target_include_directories(${EXE_NAME} 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/Digital_/Source
)

### CPP Standard
set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

### Set Work Directory
set_property(TARGET ${EXE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Resources)
set_property(TARGET ${EXE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Resources)

### Forced Includes
find_file(DEFINES_FILE_FOUND NAMES Defines.h PATHS "${CMAKE_SOURCE_DIR}/Digital_/Source/Defines")
if (DEFINES_FILE_FOUND)
    if(MSVC)
        message("Adding Force Include file(s)")
        target_compile_options(${EXE_NAME} PRIVATE /FI"${CMAKE_SOURCE_DIR}/Digital_/Source/Defines/Defines.h")
    else()
	    message("Unsupported Force Include Implementation.")
    endif()
endif()

### File Management
set_target_properties(${EXE_NAME} PROPERTIES FOLDER _MainTargets)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})
