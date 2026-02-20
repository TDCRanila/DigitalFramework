# 
set(USE_STATIC_MSVC_RUNTIME_LIBRARY OFF)

add_compile_definitions(JPH_DISABLE_CUSTOM_ALLOCATOR)

# Select X86 processor features to use (if everything is off it will be SSE2 compatible)
set(USE_SSE4_1 ON)
set(USE_SSE4_2 ON)
set(USE_AVX ON)
set(USE_AVX2 ON)
set(USE_AVX512 ON)
set(USE_LZCNT ON)
set(USE_TZCNT ON)
set(USE_F16C ON)
set(USE_FMADD ON)
