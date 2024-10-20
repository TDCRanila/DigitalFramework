# https://github.com/assimp/assimp/blob/master/Build.md
# https://github.com/assimp/assimp/wiki/FAQ#optimize-the-size-of-the-library

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# definitions
SET (BUILD_SHARED_LIBS OFF)
SET (ASSIMP_BUILD_TESTS OFF)
SET (ASSIMP_INSTALL OFF)
SET (ASSIMP_BUILD_ASSIMP_TOOLS OFF)
SET (ASSIMP_BUILD_SAMPLES OFF)
SET (ASSIMP_BUILD_TESTS OFF)
SET (ASSIMP_BUILD_ZLIB ON)

# definitons - exports
SET (ASSIMP_NO_EXPORT ON)

#definitions - imports
Set (ASSIMP_BUILD_X_IMPORTER OFF)
Set (ASSIMP_BUILD_AMF_IMPORTER OFF)
Set (ASSIMP_BUILD_3DS_IMPORTER OFF)
Set (ASSIMP_BUILD_MD3_IMPORTER OFF)
Set (ASSIMP_BUILD_MDL_IMPORTER OFF)
Set (ASSIMP_BUILD_MD2_IMPORTER OFF)
Set (ASSIMP_BUILD_PLY_IMPORTER OFF)
Set (ASSIMP_BUILD_ASE_IMPORTER OFF)
# Set (ASSIMP_BUILD_OBJ_IMPORTER OFF)
Set (ASSIMP_BUILD_HMP_IMPORTER OFF)
Set (ASSIMP_BUILD_SMD_IMPORTER OFF)
Set (ASSIMP_BUILD_MDC_IMPORTER OFF)
Set (ASSIMP_BUILD_MD5_IMPORTER OFF)
Set (ASSIMP_BUILD_STL_IMPORTER OFF)
Set (ASSIMP_BUILD_LWO_IMPORTER OFF)
Set (ASSIMP_BUILD_DXF_IMPORTER OFF)
Set (ASSIMP_BUILD_NFF_IMPORTER OFF)
Set (ASSIMP_BUILD_RAW_IMPORTER OFF)
Set (ASSIMP_BUILD_SIB_IMPORTER OFF)
Set (ASSIMP_BUILD_OFF_IMPORTER OFF)
Set (ASSIMP_BUILD_AC_IMPORTER OFF)
Set (ASSIMP_BUILD_BVH_IMPORTER OFF)
Set (ASSIMP_BUILD_IRRMESH_IMPORTER OFF)
Set (ASSIMP_BUILD_IRR_IMPORTER OFF)
Set (ASSIMP_BUILD_Q3D_IMPORTER OFF)
Set (ASSIMP_BUILD_B3D_IMPORTER OFF)
Set (ASSIMP_BUILD_COLLADA_IMPORTER OFF)
Set (ASSIMP_BUILD_TERRAGEN_IMPORTER OFF)
Set (ASSIMP_BUILD_CSM_IMPORTER OFF)
Set (ASSIMP_BUILD_3D_IMPORTER OFF)
Set (ASSIMP_BUILD_LWS_IMPORTER OFF)
Set (ASSIMP_BUILD_OGRE_IMPORTER OFF)
Set (ASSIMP_BUILD_OPENGEX_IMPORTER OFF)
Set (ASSIMP_BUILD_MS3D_IMPORTER OFF)
Set (ASSIMP_BUILD_COB_IMPORTER OFF)
Set (ASSIMP_BUILD_BLEND_IMPORTER OFF)
Set (ASSIMP_BUILD_Q3BSP_IMPORTER OFF)
Set (ASSIMP_BUILD_NDO_IMPORTER OFF)
Set (ASSIMP_BUILD_IFC_IMPORTER OFF)
Set (ASSIMP_BUILD_XGL_IMPORTER OFF)
Set (ASSIMP_BUILD_FBX_IMPORTER OFF)
Set (ASSIMP_BUILD_ASSBIN_IMPORTER OFF)
# Set (ASSIMP_BUILD_GLTF_IMPORTER OFF)
# Set (ASSIMP_BUILD_GLTF1_IMPORTER OFF)
# Set (ASSIMP_BUILD_GLTF2_IMPORTER OFF)
Set ( ASSIMP_BUILD_C4D_IMPORTER OFF)
Set ( ASSIMP_BUILD_3MF_IMPORTER OFF)
Set ( ASSIMP_BUILD_X3D_IMPORTER OFF)
Set ( ASSIMP_BUILD_MMD_IMPORTER OFF)    
Set ( ASSIMP_BUILD_M3D_IMPORTER OFF)
Set ( ASSIMP_BUILD_IQM_IMPORTER OFF)
