cmake_minimum_required(VERSION 3.25)

message("######################")
message("Configuring - DigitalTestBench\n")

### Generate a list of all Header & Source Files.
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/**/*.h")
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")
set(ALL_FILES ${HEADER_FILES} ${SOURCE_FILES})

### Executable Definition
set(EXE_NAME DigitalTestBench)

### Executable Source
add_executable(${EXE_NAME})
target_sources(${EXE_NAME}
    PRIVATE 
    ${ALL_FILES}
)

target_include_directories(${EXE_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# CPP Standard
SetCPPVersion(${EXE_NAME})

### Libraries
target_link_libraries(${EXE_NAME} PRIVATE DigitalFramework)
target_include_directories(${EXE_NAME} 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/DigitalFramework/Source
)

target_precompile_headers(${EXE_NAME} REUSE_FROM DigitalFramework)

### Set Work Directory
set_property(TARGET ${EXE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Resources)

### File Management
set_target_properties(${EXE_NAME} PROPERTIES FOLDER _MainTargets)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})
