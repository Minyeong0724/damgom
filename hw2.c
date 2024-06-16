// Assignment 2 - Efficient C Programming
// System Programming, DGIST, Prof. Yeseong Kim
// 
// YOU WILL TURN IN THIS FILE.
// Read the provided instruction carefully.


// Applying inline function

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <omp.h>

#include "bmplib.h"
#include "hw2.h"

// This implementation is simply copied from "main.c".
// Your job is to modify and optimize it for better performance!

static inline void apply_filter(Pixel* input, int nx, int ny, int width, int height, float* filter, int* r, int* g, int* b, int filter_index) {
    // 경계를 넘는 경우를 처리
    if (nx < 0 || nx >= width || ny < 0 || ny >= height) return;

    int pixel_index = nx + ny * width;
    *r += input[pixel_index].r * filter[filter_index];
    *g += input[pixel_index].g * filter[filter_index];
    *b += input[pixel_index].b * filter[filter_index];
}

static Pixel convolution(
        Pixel* input, int x, int y, int width, int height, float* filter) {
    // double r = 0;
    // double g = 0;
    // double b = 0;
    int r = 0; 
    int g = 0;
    int b = 0; 

    // trial1 || 128: 1.23 ~ 1.45 || others: 1.24
    // :memeory allocation optimization, for loop opt(y first), pre-calculate filter_index, pixel_index

    // trial2 || 128: 1.23 ~ 1.48 || 256: 1.23 ~ 1.4 || others: 1.23
    // :allocate value of r,g,b to output directly. (No use of pixel* p.)
    // +) using inline function & eliminate for loop (but same...... )

     for (int dx = -1; dx <= 1; ++dx) {
        int nx = x + dx;
        for (int dy = -1; dy <= 1; ++dy) {
            if(nx < 0 || nx >= width) continue;
            int ny = y + dy;
            if(ny < 0 || ny >= height) continue;
            int filter_index = (dx + 1) + (dy + 1) * 3;

            apply_filter(input, nx, ny, width, height, filter, &r, &g, &b, filter_index);
        }
     }
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    
    Pixel p;
    // memset(&p, 0, sizeof(p));

    p.r = (unsigned char)r;
    p.g = (unsigned char)g;
    p.b = (unsigned char)b;
    // Pixel* output;
    // output -> r = (unsigned char)r;
    // output -> g = (unsigned char)g;
    // output -> b = (unsigned char)b;

    return p;
}



void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    Pixel* input = args[2];
    Pixel* output = args[3];
    float* filter = args[4];

    // #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            output[x + y * width] = convolution(input, x, y, width, height, filter);
            // convolution(input, x, y, width, height, filter, &output[x + y * width]);
            // convolution(input, x, y, width, height, filter);
        }
    }

}
