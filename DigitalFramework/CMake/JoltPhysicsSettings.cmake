# 
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)

add_compile_definitions(JPH_DISABLE_CUSTOM_ALLOCATOR)

# Select X86 processor features to use (if everything is off it will be SSE2 compatible)
set(USE_SSE4_1 ON)
set(USE_SSE4_2 ON)
set(USE_AVX ON)
set(USE_AVX2 OFF)
set(USE_AVX512 OFF)
set(USE_LZCNT OFF)
set(USE_TZCNT OFF)
set(USE_F16C OFF)
set(USE_FMADD OFF)
