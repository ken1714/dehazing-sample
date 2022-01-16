#pragma once

#include "cuda_runtime.h"


__global__ void dehazeGPU(unsigned char* const inputValue, unsigned char* const transmission, unsigned char* dehazedValue,
                          const double minTransmission, double* const atmosphericLight,
                          const int imageWidth, const int imageHeight);
void executeDehazeGPU(unsigned char* inputValue, unsigned char* const transmission, unsigned char* dehazedValue,
                      const double minTransmission, double* const atmosphericLight,
                      const int imageWidth, const int imageHeight);
