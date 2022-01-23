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


__global__ void minimumFilterGPU(unsigned char* RGBMinimumArr, unsigned char* outputImageArr, const int radius,
                                 const int imageWidth, const int imageHeight) {
    int centerXIdx = blockIdx.x * blockDim.x + threadIdx.x;
    int centerYIdx = blockIdx.y * blockDim.y + threadIdx.y;

    if (centerXIdx >= imageWidth || centerYIdx >= imageHeight) return;

    // Define neighbor area
    int yLowerBound = (centerYIdx - radius) > 0 ? (centerYIdx - radius) : 0;
    int yUpperBound = (centerYIdx + radius) < imageHeight ? (centerYIdx + radius) : (imageHeight - 1);
    int xLowerBound = (centerXIdx - radius) > 0 ? (centerXIdx - radius) : 0;
    int xUpperBound = (centerXIdx + radius) < imageWidth ? (centerXIdx + radius) : (imageWidth - 1);

    // Get minimum value in the neighbor area
    unsigned char neighborMinimumValue = 255;
    for(int yIdx = yLowerBound; yIdx <= yUpperBound; yIdx++) {
        for (int xIdx = xLowerBound; xIdx <= xUpperBound; xIdx++) {
            unsigned char RGBMinimumValue = RGBMinimumArr[yIdx * imageWidth + xIdx];
            neighborMinimumValue = (neighborMinimumValue > RGBMinimumValue) ? RGBMinimumValue : neighborMinimumValue;
        }
    }
    outputImageArr[centerYIdx * imageWidth + centerXIdx] = neighborMinimumValue;
}


void executeDarkChannelPriorGPU(unsigned char* const inputImageArr, unsigned char* RGBMinimumArr, unsigned char* outputImageArr,
                                const int radius, const int imageWidth, const int imageHeight) {
    // Block and grid settings
    dim3 block(64, 4);
    dim3 grid((imageWidth + block.x - 1) / block.x, (imageHeight + block.y - 1) / block.y);

    calcRGBMinimumGPU<<<grid, block>>>(inputImageArr, RGBMinimumArr, imageWidth, imageHeight);

    minimumFilterGPU<<<grid, block>>>(RGBMinimumArr, outputImageArr, radius, imageWidth, imageHeight);
}
