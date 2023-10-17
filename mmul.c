#include "emscripten.h"
#include <stdlib.h> // required for malloc definition
#include <stdint.h>
#include <wasm_simd128.h>

EMSCRIPTEN_KEEPALIVE
uint8_t* alloc_buf(int n) {
  return (uint8_t *)malloc(n * n * sizeof(int) * sizeof(uint8_t));
}

EMSCRIPTEN_KEEPALIVE
void free_buf(uint8_t* p) {
  free(p);
}

EMSCRIPTEN_KEEPALIVE
void mmul(int* out, int* in_a, int* in_b, int n) {
  for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			out[i*n+j] = 0;
			for (int k = 0; k < n; k++) {
				out[i*n+j] += in_a[i*n+k] * in_b[j*n+k];
			}
		}
  }
}

EMSCRIPTEN_KEEPALIVE
void mmul_simd(int* out, int* in_a, int* in_b, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			out[i*n+j] = 0;
      int sum_arr[] = {0, 0, 0, 0};
      v128_t sum = wasm_v128_load(sum_arr);
			for (int k = 0; k < n; k+=4) {
				v128_t a = wasm_v128_load(&in_a[i*n+k]);
				v128_t b = wasm_v128_load(&in_b[j*n+k]);
				v128_t prod = wasm_i32x4_mul(a, b);
        sum = wasm_i32x4_add(sum, prod);
			}
      v128_t sum_duo = wasm_i32x4_add(sum, wasm_i32x4_shuffle(sum, sum, 2, 3, 0, 0));
      v128_t sum_one = wasm_i32x4_add(sum_duo, wasm_i32x4_shuffle(sum_duo, sum_duo, 1, 0, 0, 0));
      out[i*n+j] += wasm_i32x4_extract_lane(sum_one, 0);
		}
	}
}
