# Copy of "bgfx.cmake\cmake\bgfx\examples.cmake 'add_bgfx_shader'function."
function( AddBGFXShader FILE FOLDER )
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
