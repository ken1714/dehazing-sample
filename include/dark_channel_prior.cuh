#pragma once

#include "cuda_runtime.h"


__global__ void calcRGBMinimumGPU(unsigned char* const inputImageArr, unsigned char* RGBMinimumArr,
                                  const int imageWidth, const int imageHeight);
__global__ void minimumFilterGPU(unsigned char* RGBMinimumArr, unsigned char* outputImageArr, const int radius,
                                 const int imageWidth, const int imageHeight);
void executeDarkChannelPriorGPU(unsigned char* const inputImageArr, unsigned char* RGBMinimumArr, unsigned char* outputImageArr,
                                const int radius, const int imageWidth, const int imageHeight);
