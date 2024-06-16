// Assignment 2 - Efficient C Programming
// System Programming, DGIST, Prof. Yeseong Kim
// 
// YOU WILL TURN IN THIS FILE.
// Read the provided instruction carefully.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <omp.h>

#include "bmplib.h"
#include "hw2.h"

// This implementation is simply copied from "main.c".
// Your job is to modify and optimize it for better performance!

static Pixel convolution(
        Pixel* input, int x, int y, int width, int height, float* filter, Pixel* output) {
    // double r = 0;
    // double g = 0;
    // double b = 0;
    int r = 0;
    int g = 0;
    int b = 0;

    /*
    for (int dx = -1; dx < 2; ++dx) {
        for (int dy = -1; dy < 2; ++dy) {
            if ((y + dy) < 0 || (y + dy) >= height)
                continue;

            if ((x + dx) < 0 || (x + dx) >= width)
                continue;

            r += input[(x+dx)+(y+dy)*width].r * filter[(dx+1)+(dy+1)*3];
            g += input[(x+dx)+(y+dy)*width].g * filter[(dx+1)+(dy+1)*3];
            b += input[(x+dx)+(y+dy)*width].b * filter[(dx+1)+(dy+1)*3];
        }
    }
    */

    // TODO
    // [1] 반복 연산 local variable로 바꿔서 하기
    // [2] loop~goto label? 
    // [3] inline function
    // [4] int<< 계산 빠름(1 pixel까지 오차범위 허용) | *, / -> +, -
    // [5] for문의 끝을 알고 있으면 for 쓰지말고 걍 더하기 존내하기
    // -> 범위 계산 필요 x
    // [6] Seperate allocators << ㅇ-ㅇ.. 

    // trial1 || 128: 1.23 ~ 1.45 || others: 1.24
    // :memeory allocation optimization, for loop opt(y first), pre-calculate filter_index, pixel_index

    // trial2 || 128: 1.23 ~ 1.48 || 256: 1.23 ~ 1.4 || others: 1.23
    // :allocate value of r,g,b to output directly. (No use of pixel* p.)

    for (int dx = -1; dx <= 1; ++dx) {
        int nx = x + dx;
        for (int dy = -1; dy <= 1; ++dy) {
            if(nx < 0 || nx >= width) continue;
            int ny = y + dy;
            if(ny < 0 || ny >= height) continue;

            // if(x + dx < 0 || x + dx >= width || y + dy < 0 || y + dy >= height) continue;
            // if(y + dy < 0 || y + dy >= height) continue;

            // if(nx < 0 || nx >= width || ny < 0 || ny >= height) continue;

            int filter_index = (dx + 1) + (dy + 1) * 3;
            // int pixel_index = x + dx + (y + dy) * width;
            int pixel_index = nx + ny * width;
            // double helper = filter[filter_index];
            // r += input[pixel_index].r * helper;
            // g += input[pixel_index].g * helper;
            // b += input[pixel_index].b * helper;
            r += input[pixel_index].r * filter[filter_index];
            g += input[pixel_index].g * filter[filter_index];
            b += input[pixel_index].b * filter[filter_index];
            // r += input[(x+dx)+(y+dy)*width].r * filter[(dx+1)+(dy+1)*3];
            // g += input[(x+dx)+(y+dy)*width].g * filter[(dx+1)+(dy+1)*3];
            // b += input[(x+dx)+(y+dy)*width].b * filter[(dx+1)+(dy+1)*3];
        }
    }

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    
    // Pixel p;
    // memset(&p, 0, sizeof(p));

    // p.r = (unsigned char)r;
    // p.g = (unsigned char)g;
    // p.b = (unsigned char)b;
    // Pixel* output;
    output -> r = (unsigned char)r;
    output -> g = (unsigned char)g;
    output -> b = (unsigned char)b;

    // return p;
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
            // output[x + y * width] = convolution(input, x, y, width, height, filter);
            convolution(input, x, y, width, height, filter, &output[x + y * width]);
            // convolution(input, x, y, width, height, filter);
        }
    }

}
