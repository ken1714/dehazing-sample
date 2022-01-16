#include "remove_haze.cuh"


__global__ void dehazeGPU(unsigned char* const inputValue, unsigned char* const transmission, unsigned char* dehazedValue,
                          const double minTransmission, double* const atmosphericLight,
                          const int imageWidth, const int imageHeight) {

    int channelNum = 3;
    int xIdx = blockIdx.x * blockDim.x + threadIdx.x;
    int yIdx = blockIdx.y * blockDim.y + threadIdx.y;
    int zIdx = threadIdx.z;

    if (xIdx >= imageWidth || yIdx >= imageHeight || zIdx >= channelNum) return;

    double transmissionValue = transmission[yIdx * imageWidth + xIdx] / 255.0;
    transmissionValue = (transmissionValue > minTransmission) ? transmissionValue: minTransmission;

    double dehazed = ((double)(unsigned char)inputValue[(yIdx * imageWidth + xIdx) * channelNum + zIdx] - atmosphericLight[zIdx]) / transmissionValue + atmosphericLight[zIdx];

    if (dehazed > 255) {
        dehazed = 255;
    } else if (dehazed < 0) {
        dehazed = 0;
    }

    dehazedValue[(yIdx * imageWidth + xIdx) * channelNum + zIdx] = dehazed;
}


void executeDehazeGPU(unsigned char* const inputValue, unsigned char* const transmission, unsigned char* dehazedValue,
                      const double minTransmission, double* const atmosphericLight,
                      const int imageWidth, const int imageHeight) {
    // Block and grid settings
    int channelNum = 3;
    dim3 block(64, 4, channelNum);
    dim3 grid((imageWidth + block.x - 1) / block.x, (imageHeight + block.y - 1) / block.y);

    // Copy atmospheric light from host to device
    double *dAtmosphericLight;
    int atmosphericLightMemSize = sizeof(double) * channelNum;
    cudaMalloc((void **)&dAtmosphericLight, atmosphericLightMemSize);
    cudaMemcpy(dAtmosphericLight, atmosphericLight, atmosphericLightMemSize, cudaMemcpyHostToDevice);

    dehazeGPU<<<grid, block>>>(inputValue, transmission, dehazedValue, minTransmission, dAtmosphericLight, imageWidth, imageHeight);
}
