cmake_minimum_required(VERSION 3.17.2)

message("######################")
message("Configuring - DigitalGame\n")

### Generate a list of all Header & Source Files.
execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Tools/GenerateSourceFilesList.bat ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_source.txt SOURCE_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_header.txt HEADER_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_all.txt ALL_FILES)
string(REPLACE "\\" "/" SOURCE_FILES    "${SOURCE_FILES}")
string(REPLACE "\\" "/" HEADER_FILES    "${HEADER_FILES}")
string(REPLACE "\\" "/" ALL_FILES       "${ALL_FILES}")
    
### Executable Definition
set(EXE_NAME DigitalGame)
project(${EXE_NAME} VERSION 1.0.0)

# Executable Source
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

### Shader Managements
include(${CMAKE_SOURCE_DIR}/DigitalFramework/CMake/BGFXUtility.cmake)

# Gather Shaders
execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Tools/GenerateShaderFileList.bat ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR})
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_shader.txt SHADER_FILES)
string(REPLACE "\\" "/" SHADER_FILES "${SHADER_FILES}")

# Configure Shaders
if( NOT ARG_COMMON AND NOT IOS AND NOT EMSCRIPTEN)
    foreach(SHADER ${SHADER_FILES})
		message("Adding Shader to build pipeline: " + ${SHADER})
        AddBGFXShader(${SHADER} ${CMAKE_SOURCE_DIR}/Resources/shaders)
    endforeach()
endif()

# Make sure to add {SHADER_FILES} as dependency for the target.
target_sources(${EXE_NAME} PRIVATE ${SHADER_FILES})

# Group Shaders (VS)
source_group("Shader Files" FILES ${SHADER_FILES})

### Set Work Directory
set_property(TARGET ${EXE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Resources)

### File Management
set_target_properties(${EXE_NAME} PROPERTIES FOLDER _MainTargets)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})
