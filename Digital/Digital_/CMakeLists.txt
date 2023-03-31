cmake_minimum_required(VERSION 3.17.2)

### Generate a list of all Header & Source Files.
execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Tools/GenerateSourceFilesList.bat ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_source.txt SOURCE_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_header.txt HEADER_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_all.txt ALL_FILES)
string(REPLACE "\\" "/" SOURCE_FILES    "${SOURCE_FILES}")
string(REPLACE "\\" "/" HEADER_FILES    "${HEADER_FILES}")
string(REPLACE "\\" "/" ALL_FILES       "${ALL_FILES}")

### Library Definition
set(LIB_NAME Digital_)
project(${LIB_NAME} VERSION 1.0.0)

add_library(${LIB_NAME} STATIC)
add_library(LIB:${LIB_NAME} ALIAS ${LIB_NAME})
target_sources(${LIB_NAME}
    PRIVATE 
    ${ALL_FILES}
)

target_include_directories(${LIB_NAME} 
    PRIVATE
    # Internal Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    PUBLIC 
    # External headers for top-level projects.
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    # External Headers for external projects.
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

### CPP Standard
SetCPPVersion(${LIB_NAME})

### Set Work Directory
set_property(TARGET ${LIB_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Resources)

### PreCompiled Headers
find_file(PCH_FILE_FOUND NAMES DFWPCH.h PATHS "${CMAKE_SOURCE_DIR}/Digital_/Source/DFW")
if (PCH_FILE_FOUND)
    if(MSVC)
        target_precompile_headers(${LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Digital_/Source/DFW/DFWPCH.h")
	    message("Adding PCH file(s)")
    else()
	    message("Unsupported PCH Implementation.")
    endif()
else()
	message("Cannot Find PCH file(s)")
endif()

### Forced Includes
find_file(DEFINES_FILE_FOUND NAMES Defines.h PATHS "${CMAKE_SOURCE_DIR}/Digital_/Source/Defines")
if (DEFINES_FILE_FOUND)
    if(MSVC)
        message("Adding Force Include file(s)")
        target_compile_options(${LIB_NAME} PRIVATE /FI"${CMAKE_SOURCE_DIR}/Digital_/Source/Defines/Defines.h")
    else()
	    message("Unsupported Force Include Implementation.")
    endif()
endif()

### File Management
set_target_properties(${LIB_NAME} PROPERTIES FOLDER _MainTargets)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})


### EXTERNALS/LIBRARIES
macro(AnnounceLibraryAddition LibraryName)
    message("\n----------------------")
    message("Adding Library: ${LibraryName}")
endmacro()

message("\n----------------------")
message("Configuring Libraries & Includes:")

## bgfx.cmake
AnnounceLibraryAddition("bgfx")
set(BGFX_CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake)
set(BGFX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake/bgfx/include)
set(BIMG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake/bimg/include)
set(BX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake/bx/include)
set(BGFX_LIB_NAME bgfx)

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake bgfx.cmake)

target_link_libraries(${LIB_NAME} ${BGFX_LIB_NAME})
target_include_directories(${LIB_NAME} PUBLIC "${BGFX_INCLUDE_DIR}")
target_include_directories(${LIB_NAME} PUBLIC "${BIMG_INCLUDE_DIR}")
target_include_directories(${LIB_NAME} PUBLIC "${BX_INCLUDE_DIR}")

target_compile_definitions(bx PUBLIC 
    "$<$<CONFIG:PreProcessorDebug>:BX_CONFIG_DEBUG=1>"
    "$<$<CONFIG:PreRelease>:BX_CONFIG_DEBUG=1>")

## entt
AnnounceLibraryAddition("entt")
set(ENTT_CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/entt)
set(ENTT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/entt/src)

target_include_directories(${LIB_NAME} PUBLIC "${ENTT_INCLUDE_DIR}")

## imgui
AnnounceLibraryAddition("Dear ImGui")
set(IMGUI_CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/imgui)
set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/imgui)
set(IMGUI_LIB_NAME imgui)

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/imgui imgui)

target_link_libraries(${LIB_NAME} ${IMGUI_LIB_NAME})
target_include_directories(${LIB_NAME} PUBLIC "${IMGUI_INCLUDE_DIR}")

## glfw
AnnounceLibraryAddition("glfw")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/glfw/include)
set(GLFW_LIB_NAME glfw)

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/glfw ${GLFW_LIB_NAME})

target_link_libraries(${LIB_NAME} ${GLFW_LIB_NAME})
target_include_directories(${LIB_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")

## GLM - https://github.com/g-truc/glm.git
AnnounceLibraryAddition("glm")
set(GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/glm)

target_include_directories(${LIB_NAME} PUBLIC "${GLM_INCLUDE_DIR}")

## spdlog
AnnounceLibraryAddition("spdlog")
set(SPDLOG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/spdlog/include)
set(SPDLOG_LIB_NAME spdlog)

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/spdlog spdlog)

target_include_directories(${LIB_NAME} PUBLIC "${SPDLOG_INCLUDE_DIR}")

set_target_properties(${SPDLOG_LIB_NAME} PROPERTIES FOLDER "${SPDLOG_LIB_NAME}")

## assimp
AnnounceLibraryAddition("assimp")

include(CMake/ASSIMPSettings.cmake)

set(ASSIMP_LIB_NAME assimp)
set(ASSIMP_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/assimp)
set(ASSIMP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/assimp/include)
set(ASSIMP_INCLUDE_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(${ASSIMP_DIR} ${ASSIMP_LIB_NAME})

target_link_libraries(${LIB_NAME} ${ASSIMP_LIB_NAME})
target_include_directories(${LIB_NAME} PUBLIC ${ASSIMP_INCLUDE_DIR} ${ASSIMP_INCLUDE_GEN_DIR})

set_target_properties(${ASSIMP_LIB_NAME}  PROPERTIES FOLDER ${ASSIMP_LIB_NAME})
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER ${ASSIMP_LIB_NAME})
set_target_properties(zlibstatic PROPERTIES FOLDER ${ASSIMP_LIB_NAME})

include(CMake/JoltPhysicsSettings.cmake)

message("Adding Submdoule: JoltPhysics")

set(JOLT_LIB_NAME Jolt)
set(JOLT_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/JoltPhysics)
set(JOLT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/JoltPhysics)
set(JOLT_INCLUDE_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR})

find_path(JOLT_FOUND NAMES "Jolt.h" PATHS "${JOLT_INCLUDE_DIR}/Jolt")
if((NOT JOLT_FOUND) OR (NOT EXISTS ${JOLT_INCLUDE_DIR}))
message("Unable to find Submodule: ${JOLT_LIB_NAME} or file Jolt.h")
execute_process(COMMAND git submodule update --init -- ${JOLT_INCLUDE_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
message("Found Submodule: ${JOLT_LIB_NAME}")
endif()

add_subdirectory(${JOLT_DIR}/Build ${JOLT_LIB_NAME})
target_link_libraries(${LIB_NAME} ${JOLT_LIB_NAME})
set_target_properties(${JOLT_LIB_NAME}  PROPERTIES FOLDER ${JOLT_LIB_NAME})
target_include_directories(${LIB_NAME} PUBLIC ${JOLT_INCLUDE_DIR} ${JOLT_INCLUDE_GEN_DIR})

message("\n")

## Boost - 
AnnounceLibraryAddition("boost")
set(BOOST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/boost)

target_include_directories(${LIB_NAME} PUBLIC ${BOOST_INCLUDE_DIR})

message("\n")

# stb
AnnounceLibraryAddition("stb")
set (STB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/stb)

target_include_directories(${LIB_NAME} PUBLIC ${STB_INCLUDE_DIR})
