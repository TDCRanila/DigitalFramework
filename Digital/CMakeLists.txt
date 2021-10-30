cmake_minimum_required(VERSION 3.17.2)

set(PROJECT_NAME ProjectSolution)

project(${PROJECT_NAME})

### Project Properties.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_PredfinedTargetsFolder")

### Platform Specific Defines
if(WIN32 AND NOT UNIX AND NOT APPLE)
    add_compile_definitions(DFW_PLATFORM_WINDOWS)
elseif(UNIX AND NOT WIN32 AND NOT APPLE)
    add_compile_definitions(DFW_PLATFORM_UNIX)
else()
    add_compile_definitions(DFW_PLATFORM_UNSUPPORTED)
endif()

### Build Configurations.
if (MSVC)
    if(CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_CONFIGURATION_TYPES PreProcessorDebug Debug PreRelease Release Final)
        set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}")
    endif()

    # PreProcessorDebug Config
    set(CMAKE_CXX_FLAGS_PREPROCESSORDEBUG "/W4 /Zi /Od /Ob0 /RTC1 /MDd /P /DDFW_PDEBUG64")
    set(CMAKE_EXE_LINKER_FLAGS_PREPROCESSORDEBUG "/DEBUG:FULL")

    # Debug Config
    set(CMAKE_CXX_FLAGS_DEBUG "/W4 /Zi /Od /Ob0 /RTC1 /MDd /DDFW_DEBUG64")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FULL")

    # PreRelease Config
    set(CMAKE_CXX_FLAGS_PRERELEASE "/W4 /Zi /Od /Ob0 /RTC1 /MDd /DDFW_PRERELEASE64")
    set(CMAKE_EXE_LINKER_FLAGS_PRERELEASE "/DEBUG:FASTLINK")

    # Release Config
    set(CMAKE_CXX_FLAGS_RELEASE "/W4 /WX /O2 /Oi /Ot /MD /DDFW_RELEASE64")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")

    # Final Config
    set(CMAKE_CXX_FLAGS_FINAL "/W4 /WX /O2 /Oi /Ot /MD /DDFW_FINAL64")
    set(CMAKE_EXE_LINKER_FLAGS_FINAL "")
else()
message("Unsupported Build Configurations.")
endif()

add_subdirectory(Digital_)
add_subdirectory(DigitalGame_)
add_subdirectory(DigitalTestBench_)
