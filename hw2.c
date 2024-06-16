#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <omp.h>
#include "bmplib.h"
#include "hw2.h"

// Convolution 함수
static void convolution(Pixel* input, int x, int y, int width, int height, float* filter, Pixel* output) {
    double r = 0;
    double g = 0;
    double b = 0;

    int nx, ny, pixel_index, filter_index;

    // Top-left
    nx = x - 1; ny = y - 1;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 0;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // Top-middle
    nx = x; ny = y - 1;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 1;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // Top-right
    nx = x + 1; ny = y - 1;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 2;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // Middle-left
    nx = x - 1; ny = y;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 3;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // Middle-middle
    nx = x; ny = y;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 4;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // Middle-right
    nx = x + 1; ny = y;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 5;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // Bottom-left
    nx = x - 1; ny = y + 1;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 6;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // Bottom-middle
    nx = x; ny = y + 1;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 7;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // Bottom-right
    nx = x + 1; ny = y + 1;
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
        filter_index = 8;
        pixel_index = nx + ny * width;
        r += input[pixel_index].r * filter[filter_index];
        g += input[pixel_index].g * filter[filter_index];
        b += input[pixel_index].b * filter[filter_index];
    }

    // 값 범위를 0-255 사이로 클램핑
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    // 결과를 output 배열에 직접 저장
    output->r = (unsigned char)r;
    output->g = (unsigned char)g;
    output->b = (unsigned char)b;
}

// 최적화된 필터 함수 (OpenMP 사용)
void filter_optimized(void* args[]) {
    unsigned int width = *(unsigned int*)args[0];
    unsigned int height = *(unsigned int*)args[1];
    Pixel* input = args[2];
    Pixel* output = args[3];
    float* filter = args[4];

    // OpenMP를 사용하여 병렬 처리
    // #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            convolution(input, x, y, width, height, filter, &output[x + y * width]);
        }
    }
}
