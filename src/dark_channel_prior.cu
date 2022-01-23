#include "dark_channel_prior.cuh"

__global__ void calcRGBMinimumGPU(unsigned char* const inputImageArr, unsigned char* RGBMinimumArr,
                                  const int imageWidth, const int imageHeight) {

    int channelNum = 3;
    int xIdx = blockIdx.x * blockDim.x + threadIdx.x;
    int yIdx = blockIdx.y * blockDim.y + threadIdx.y;

    if (xIdx >= imageWidth || yIdx >= imageHeight) return;

    unsigned char b = inputImageArr[(yIdx * imageWidth + xIdx) * channelNum];
    unsigned char g = inputImageArr[(yIdx * imageWidth + xIdx) * channelNum + 1];
    unsigned char r = inputImageArr[(yIdx * imageWidth + xIdx) * channelNum + 2];

    unsigned char RGBMinimumValue;
    RGBMinimumValue = (b < g) ? b : g;
    RGBMinimumValue = (RGBMinimumValue < r) ? RGBMinimumValue : r;

    RGBMinimumArr[yIdx * imageWidth + xIdx] = RGBMinimumValue;
}


void executeDarkChannelPriorGPU(unsigned char* const inputImageArr, unsigned char* RGBMinimumArr,
                                const int imageWidth, const int imageHeight) {
    // Block and grid settings
    dim3 block(64, 4);
    dim3 grid((imageWidth + block.x - 1) / block.x, (imageHeight + block.y - 1) / block.y);

    calcRGBMinimumGPU<<<grid, block>>>(inputImageArr, RGBMinimumArr, imageWidth, imageHeight);
}
