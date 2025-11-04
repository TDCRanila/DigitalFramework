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

add_subdirectory(DigitalFramework)
add_subdirectory(ExampleProjects/DigitalGame)
add_subdirectory(ExampleProjects/DigitalTestBench)
