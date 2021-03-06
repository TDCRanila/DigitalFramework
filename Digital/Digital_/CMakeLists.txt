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

### Set Work Directory
set_property(TARGET ${LIB_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Resources)

# target_include_directories(${LIB_NAME} PUBLIC ${HEADER_FILES})
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${LIB_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_target_properties(${LIB_NAME} PROPERTIES FOLDER _MainTargets)

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
        target_compile_options(${LIB_NAME} PRIVATE /FI"${CMAKE_SOURCE_DIR}/Digital_/Source/Defines/Defines.h")
	message("Adding Force Include file(s)")
    else()
	message("Unsupported Force Include Implementation.")
    endif()
endif()

message("\nConfiguring Subodules & Includes:\n")

### bgfx.cmake
message("Adding SubModule: bgfx")
set(BGFX_CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake)
set(BGFX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake/bgfx/include)
set(BIMG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake/bimg/include)
set(BX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake/bx/include)
set(BGFX_LIB_NAME bgfx)

find_path(BGFX_FOUND NAMES "bgfx.h" PATHS "${BGFX_INCLUDE_DIR}/bgfx")
if((NOT BGFX_FOUND) OR (NOT EXISTS ${BGFX_INCLUDE_DIR}))
    message("Unable to find Submodule: ${BGFX_LIB_NAME} or file bgfx.hpp")
    execute_process(COMMAND git submodule update --init -- ${BGFX_CMAKE_INCLUDE_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Found Submodule: ${BGFX_LIB_NAME}")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/bgfx.cmake bgfx.cmake)

message("\n")

target_link_libraries(${LIB_NAME} bgfx)
target_include_directories(${LIB_NAME} PUBLIC "${BGFX_INCLUDE_DIR}")
#target_include_directories(${LIB_NAME} PUBLIC "${BIMG_INCLUDE_DIR}")
#target_include_directories(${LIB_NAME} PUBLIC "${BX_INCLUDE_DIR}")

### digital-imgui
message("Adding SubModule: Digital/Dear ImGui")
set(IMGUI_CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/digital-imgui)
set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/digital-imgui)
set(IMGUI_LIB_NAME imgui)

find_path(IMGUI_FOUND NAMES "imgui.h" PATHS "${IMGUI_INCLUDE_DIR}/imgui")
if((NOT IMGUI_FOUND) OR (NOT EXISTS ${IMGUI_INCLUDE_DIR}))
    message("Unable to find Submodule: ${IMGUI_LIB_NAME} or file imgui.h")
    execute_process(COMMAND git submodule update --init -- ${IMGUI_CMAKE_INCLUDE_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Found Submodule: ${IMGUI_LIB_NAME}")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/digital-imgui digital-imgui)

target_link_libraries(${LIB_NAME} imgui)
target_include_directories(${LIB_NAME} PUBLIC "${IMGUI_INCLUDE_DIR}")

message("\n")

### glfw
message("Adding Submodule: glfw")
set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/glfw/include)
set(GLFW_LIB_NAME glfw)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_path(GLFW_FOUND NAMES "glfw3.h" PATHS "${GLFW_INCLUDE_DIR}/GLFW")
if((NOT GLFW_FOUND) OR (NOT EXISTS ${GLFW_INCLUDE_DIR}))
    message("Unable to find Submodule: ${GLFW_LIB_NAME} or file glfw3.hpp")
    execute_process(COMMAND git submodule update --init -- ${GLM_INCLUDE_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Found Submodule: ${GLFW_LIB_NAME}")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/glfw glfw)

target_link_libraries(${LIB_NAME} glfw)
target_include_directories(${LIB_NAME} PUBLIC "${GLFW_INCLUDE_DIR}")

message("\n")

### GLM - https://github.com/g-truc/glm.git
message("Adding Submodule: glm")
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

message("\n")

### spdlog
message("Adding Submodule: spdlog")
set(SPDLOG_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/External/spdlog)
set(SPDLOG_LIB_NAME spdlog)
find_path(SPDLOG_FOUND NAMES "spdlog.h" PATHS "${SPDLOG_INCLUDE_DIR}/include/spdlog")
if((NOT SPDLOG_FOUND) OR (NOT EXISTS ${SPDLOG_INCLUDE_DIR}))
    message("Unable to find Submodule: ${SPDLOG_LIB_NAME} or file spdlog.h")
    execute_process(COMMAND git submodule update --init -- ${SPDLOG_INCLUDE_DIR} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
else()
message("Found Submodule: ${SPDLOG_LIB_NAME}")
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependency/External/spdlog spdlog)
set_target_properties(spdlog  PROPERTIES FOLDER spdlog)

target_include_directories(${LIB_NAME} PUBLIC "${SPDLOG_INCLUDE_DIR}/include")

message("\n")

### Boost - 
message("Adding Files: boost")
set(BOOST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependency/boost)
set(BOOST_LIB_NAME Boost)
target_include_directories(${LIB_NAME} PUBLIC "${BOOST_INCLUDE_DIR}")

### Project Filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})

