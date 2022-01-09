#include "remove_haze.cuh"
#include <stdio.h>


__global__ void dehazeGPU(unsigned char* const inputValue, unsigned char* const transmission, unsigned char* dehazedValue,
                          const double minTransmission, const double atmosphericB, const double atmosphericG, const double atmosphericR,
                          const int imageWidth, const int imageHeight) {

    int channelNum = 3;
    int xIdx = blockIdx.x * blockDim.x + threadIdx.x;
    int yIdx = blockIdx.y * blockDim.y + threadIdx.y;

    if (xIdx >= imageWidth || yIdx >= imageHeight) return;

    double transmissionValue = transmission[yIdx * imageWidth + xIdx] / 255.0;
    transmissionValue = (transmissionValue > minTransmission) ? transmissionValue: minTransmission;

    double dehazedB = ((double)(unsigned char)inputValue[(yIdx * imageWidth + xIdx) * channelNum + 0] - atmosphericB) / transmissionValue + atmosphericB;
    double dehazedG = ((double)(unsigned char)inputValue[(yIdx * imageWidth + xIdx) * channelNum + 1] - atmosphericG) / transmissionValue + atmosphericG;
    double dehazedR = ((double)(unsigned char)inputValue[(yIdx * imageWidth + xIdx) * channelNum + 2] - atmosphericR) / transmissionValue + atmosphericR;

    if (dehazedB > 255) {
        dehazedB = 255;
    } else if (dehazedB < 0) {
        dehazedB = 0;
    }

    if (dehazedG > 255) {
        dehazedG = 255;
    } else if (dehazedG < 0) {
        dehazedG = 0;
    }

    if (dehazedR > 255) {
        dehazedR = 255;
    } else if (dehazedR < 0) {
        dehazedR = 0;
    }

    dehazedValue[(yIdx * imageWidth + xIdx) * channelNum + 0] = dehazedB;
    dehazedValue[(yIdx * imageWidth + xIdx) * channelNum + 1] = dehazedG;
    dehazedValue[(yIdx * imageWidth + xIdx) * channelNum + 2] = dehazedR;
}


void executeDehazeGPU(unsigned char* const inputValue, unsigned char* const transmission, unsigned char* dehazedValue,
                      const double minTransmission, const double atmosphericB, const double atmosphericG, const double atmosphericR,
                      const int imageWidth, const int imageHeight) {

    dim3 block(64, 4);
    dim3 grid((imageWidth + block.x - 1) / block.x, (imageHeight + block.y - 1) / block.y);

    dehazeGPU<<<grid, block>>>(inputValue, transmission, dehazedValue, minTransmission, atmosphericB, atmosphericG, atmosphericR, imageWidth, imageHeight);
}
