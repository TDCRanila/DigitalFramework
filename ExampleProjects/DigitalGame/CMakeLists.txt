cmake_minimum_required(VERSION 3.17.2)

### Generate a list of all Header & Source Files.
execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Tools/GenerateSourceFilesList.bat ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_source.txt SOURCE_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_header.txt HEADER_FILES)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_all.txt ALL_FILES)
string(REPLACE "\\" "/" SOURCE_FILES    "${SOURCE_FILES}")
string(REPLACE "\\" "/" HEADER_FILES    "${HEADER_FILES}")
string(REPLACE "\\" "/" ALL_FILES       "${ALL_FILES}")

### Shader Management
function( add_bgfx_shader FILE FOLDER )
	get_filename_component( FILENAME "${FILE}" NAME_WE )
	string( SUBSTRING "${FILENAME}" 0 2 TYPE )
	if( "${TYPE}" STREQUAL "fs" )
		set( TYPE "FRAGMENT" )
		set( D3D_PREFIX "s" )
	elseif( "${TYPE}" STREQUAL "vs" )
		set( TYPE "VERTEX" )
		set( D3D_PREFIX "s" )
	elseif( "${TYPE}" STREQUAL "cs" )
		set( TYPE "COMPUTE" )
 		set( D3D_PREFIX "s" )
	else()
		set( TYPE "" )
	endif()

	if( NOT "${TYPE}" STREQUAL "" )
		set( COMMON FILE ${FILE} ${TYPE} INCLUDES ${BGFX_DIR}/src )
		set( OUTPUTS "" )
		set( OUTPUTS_PRETTY "" )

		if( WIN32 )
			# dx11
			set( DX11_OUTPUT ${FOLDER}/dx11/${FILENAME}.bin )
			if( NOT "${TYPE}" STREQUAL "COMPUTE" )
				_bgfx_shaderc_parse ( DX11 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_5_0 O 3 OUTPUT ${DX11_OUTPUT} )
			else()
				_bgfx_shaderc_parse ( DX11 ${COMMON} WINDOWS PROFILE ${D3D_PREFIX}_5_0 O 1 OUTPUT ${DX11_OUTPUT} )
			endif()
			list( APPEND OUTPUTS "DX11" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX11, " )
		endif()

		if( APPLE )
			# metal
			set( METAL_OUTPUT ${FOLDER}/metal/${FILENAME}.bin )
			_bgfx_shaderc_parse ( METAL ${COMMON} OSX PROFILE metal OUTPUT ${METAL_OUTPUT} )
			list( APPEND OUTPUTS "METAL" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}Metal, " )
		endif()

		# essl
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			set( ESSL_OUTPUT ${FOLDER}/essl/${FILENAME}.bin )
			_bgfx_shaderc_parse ( ESSL ${COMMON} ANDROID PROFILE 100_es OUTPUT ${ESSL_OUTPUT} )
			list( APPEND OUTPUTS "ESSL" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}ESSL, " )
		endif()

		# glsl
		set( GLSL_OUTPUT ${FOLDER}/glsl/${FILENAME}.bin )
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			_bgfx_shaderc_parse ( GLSL ${COMMON} LINUX PROFILE 140 OUTPUT ${GLSL_OUTPUT} )
		else()
			_bgfx_shaderc_parse ( GLSL ${COMMON} LINUX PROFILE 430 OUTPUT ${GLSL_OUTPUT} )
		endif()
		list( APPEND OUTPUTS "GLSL" )
		set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}GLSL, " )

		# spirv
		if( NOT "${TYPE}" STREQUAL "COMPUTE" )
			set( SPIRV_OUTPUT ${FOLDER}/spirv/${FILENAME}.bin )
			_bgfx_shaderc_parse ( SPIRV ${COMMON} LINUX PROFILE spirv OUTPUT ${SPIRV_OUTPUT} )
			list( APPEND OUTPUTS "SPIRV" )
			set( OUTPUTS_PRETTY "${OUTPUTS_PRETTY}SPIRV" )
			set( OUTPUT_FILES "" )
			set( COMMANDS "" )
		endif()

		foreach( OUT ${OUTPUTS} )
			list( APPEND OUTPUT_FILES ${${OUT}_OUTPUT} )
			list( APPEND OUTPUT_FILES_PRETTY "\n" ${${OUT}_OUTPUT} " - " )
			list( APPEND COMMANDS COMMAND "$<TARGET_FILE:shaderc>" ${${OUT}} )
			get_filename_component( OUT_DIR ${${OUT}_OUTPUT} DIRECTORY )
			file( MAKE_DIRECTORY ${OUT_DIR} )
		endforeach()

		file( RELATIVE_PATH PRINT_NAME ${FOLDER} ${FILE} )
        message("OUTPUT_FILES: " + ${OUTPUT_FILES_PRETTY})
		add_custom_command(
			MAIN_DEPENDENCY ${FILE}
			OUTPUT ${OUTPUT_FILES}
			${COMMANDS}
			COMMENT "Compiling shader ${PRINT_NAME} for ${OUTPUTS_PRETTY}"
		)
	endif()
endfunction()

# Gather Shaders
execute_process(COMMAND ${CMAKE_SOURCE_DIR}/Tools/GenerateShaderFileList.bat ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR})
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/files_shader.txt SHADER_FILES)
string(REPLACE "\\" "/" SHADER_FILES "${SHADER_FILES}")

# Configure Shaders
if( NOT ARG_COMMON AND NOT IOS AND NOT EMSCRIPTEN)
    foreach(SHADER ${SHADER_FILES})
		message("\nAdding Shader to build pipeline: " + ${SHADER})
        add_bgfx_shader(${SHADER} ${CMAKE_SOURCE_DIR}/Resources/shaders)
    endforeach()
endif()
    
# Group Shaders (VS)
source_group("Shader Files" FILES ${SHADER_FILES})

### Executable Definition
set(EXE_NAME DigitalGame)
project(${EXE_NAME} VERSION 1.0.0)

# Make sure to make {SHADER_FILES} a dependency for the target.
add_executable(${EXE_NAME} ${SHADER_FILES})
target_sources(${EXE_NAME}
    PRIVATE 
    ${ALL_FILES}
)

target_include_directories(${EXE_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(${EXE_NAME} PRIVATE DigitalFramework)
target_include_directories(${EXE_NAME} 
    PRIVATE 
    ${CMAKE_SOURCE_DIR}/DigitalFramework/Source
)

target_precompile_headers(${EXE_NAME} REUSE_FROM DigitalFramework)

### CPP Standard
SetCPPVersion(${EXE_NAME})

### Set Work Directory
set_property(TARGET ${EXE_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Resources)

### File Management
set_target_properties(${EXE_NAME} PROPERTIES FOLDER _MainTargets)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})
