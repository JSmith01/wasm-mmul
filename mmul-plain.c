#include "emscripten.h"
#include <stdlib.h> // required for malloc definition
#include <stdint.h>

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
