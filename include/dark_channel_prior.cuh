#pragma once

#include "cuda_runtime.h"


__global__ void calcRGBMinimumGPU(unsigned char* const inputImageArr, unsigned char* RGBMinimumArr,
                                  const int imageWidth, const int imageHeight);
void executeDarkChannelPriorGPU(unsigned char* const inputImageArr, unsigned char* RGBMinimumArr,
                                const int imageWidth, const int imageHeight);
