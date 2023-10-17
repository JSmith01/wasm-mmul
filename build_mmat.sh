#!/bin/sh

emcc -O3 -Oz -msimd128 -sALLOW_MEMORY_GROWTH -s EXPORT_NAME=loadWasmMmul -sEXPORTED_FUNCTIONS=_alloc_buf,_free_buf,_mmul,_mmul_simd mmul.c -o mmul.mjs