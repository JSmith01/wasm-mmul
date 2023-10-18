#!/bin/sh

emcc -O3 -msimd128 -sALLOW_MEMORY_GROWTH -sTOTAL_MEMORY=240MB \
    -s EXPORT_NAME=loadWasmMmul \
    -sEXPORTED_FUNCTIONS=_alloc_buf,_free_buf,_mmul,_mmul_simd \
    mmul.c -o mmul.mjs
emcc -O3 -sALLOW_MEMORY_GROWTH -sTOTAL_MEMORY=240MB \
    -s EXPORT_NAME=loadWasmMmul \
    -sEXPORTED_FUNCTIONS=_alloc_buf,_free_buf,_mmul \
    mmul-plain.c -o mmul-plain.mjs
