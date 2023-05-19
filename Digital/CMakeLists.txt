cmake_minimum_required(VERSION 3.17.2)

include(Utility.cmake)

macro(SetCPPVersion Target)
set_property(TARGET ${Target} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${Target} PROPERTY CXX_STANDARD_REQUIRED ON)
endmacro()

### Project.
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
        set(CMAKE_CONFIGURATION_TYPES PreProcessorDebug;Debug;PreRelease;Release)
        set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}")
    endif()

    # PreProcessorDebug Config -  WarningLVL4, No Optimization, PDB, RunTimeErrorChecks, PreprocessingOutput, MultiProccesor
    set(CMAKE_CXX_FLAGS_PREPROCESSORDEBUG "/W4 /Od /Ob0 /Zi /Zf /RTC1 /P /MP /DDFW_PDEBUG64")
    set(CMAKE_EXE_LINKER_FLAGS_PREPROCESSORDEBUG "/DEBUG:FASTLINK")

    # Debug Config - WarningLVL4, No Optimization, PDB, RunTimeErrorChecks, MultiProccesor
    set(CMAKE_CXX_FLAGS_DEBUG "/W4 /Od /Ob0 /Zi /Zf /RTC1 /MP /DDFW_DEBUG64")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FASTLINK")

    # PreRelease Config - WarningLVL4, Speed Optimized, DebugForOptimized, MultiProccesor
    set(CMAKE_CXX_FLAGS_PRERELEASE "/W4 /O2 /Ob0 /Zo /MP /DDFW_PRERELEASE64")
    set(CMAKE_EXE_LINKER_FLAGS_PRERELEASE "/DEBUG:FASTLINK")

    # Release Config - WarningLVL4, Warning=Error, Speed Optimized, WholeProgramOptimization, MultiProccesor
    set(CMAKE_CXX_FLAGS_RELEASE "/W4 /WX /O2 /Oi /GL /MP /DDFW_RELEASE64")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO /LTCG:INCREMENTAL")
else()
    message("Unsupported Build Configurations.")
endif()

add_subdirectory(Digital_)
add_subdirectory(DigitalGame_)
add_subdirectory(DigitalTestBench_)
