cmake_minimum_required(VERSION 3.17.2)

project(imgui LANGUAGES CXX)

set(IMGUI_CMAKE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(IMGUI_RESOURCE_CMAKE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui-resources)
set(IMGUI_USER_CMAKE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui-user)

set(IMGUI_HEADER_FILES 
    ${IMGUI_CMAKE_INCLUDE_DIR}/imgui.h
    ${IMGUI_CMAKE_INCLUDE_DIR}/imconfig.h
    ${IMGUI_CMAKE_INCLUDE_DIR}/imgui_internal.h
    ${IMGUI_CMAKE_INCLUDE_DIR}/imstb_rectpack.h
    ${IMGUI_CMAKE_INCLUDE_DIR}/imstb_textedit.h
    ${IMGUI_CMAKE_INCLUDE_DIR}/imstb_truetype.h
)

set(IMGUI_SOURCE_FILES
    ${IMGUI_CMAKE_INCLUDE_DIR}/imgui.cpp
    ${IMGUI_CMAKE_INCLUDE_DIR}/imgui_demo.cpp
    ${IMGUI_CMAKE_INCLUDE_DIR}/imgui_draw.cpp
    ${IMGUI_CMAKE_INCLUDE_DIR}/imgui_tables.cpp
    ${IMGUI_CMAKE_INCLUDE_DIR}/imgui_widgets.cpp
)

set(IMGUI_CUSTOM_USER_FILE 
    ${IMGUI_USER_CMAKE_INCLUDE_DIR}/imgui_user.h
)

set(IMGUI_RESOURCES_FILES
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/fs_imgui_image.bin.h
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/fs_ocornut_imgui.bin.h
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/vs_imgui_image.bin.h
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/vs_ocornut_imgui.bin.h
    
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/icons_font_awesome.h
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/icons_font_awesome.ttf.h
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/icons_kenney.h
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/icons_kenney.ttf.h
    
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/roboto_regular.ttf.h
    ${IMGUI_RESOURCE_CMAKE_INCLUDE_DIR}/robotomono_regular.ttf.h
)

add_library(imgui
    ${IMGUI_HEADER_FILES}
    ${IMGUI_SOURCE_FILES}
    ${IMGUI_CUSTOM_USER_FILE}
    ${IMGUI_RESOURCES_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${IMGUI_CMAKE_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ImGui)

source_group("Header Files" FILES ${IMGUI_HEADER_FILES})
source_group("Source Files" FILES ${IMGUI_SOURCE_FILES})
source_group("User Files" FILES ${IMGUI_CUSTOM_USER_FILE})
source_group("Resource Files" FILES ${IMGUI_RESOURCES_FILES} )
