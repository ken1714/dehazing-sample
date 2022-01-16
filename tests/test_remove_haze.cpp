#include "gtest/gtest.h"
#include "remove_haze.hpp"
#include "remove_haze.cuh"
#include "utils.hpp"

// Path
const std::string inputImagePath = "data/original.png";
const std::string transmissionPath = "data/transmission_map.png";
const std::string referenceImagePath = "data/dehazed.png";

TEST(RemoveHazeTest, RemoveHaze) {
    // Read images
    const cv::Mat inputImage = cv::imread(inputImagePath);
    const cv::Mat transmissionMap = cv::imread(transmissionPath, cv::IMREAD_GRAYSCALE);
    const cv::Mat referenceImage = cv::imread(referenceImagePath);
    cv::Mat outputImage;

    // Execute
    const double minTransmission = 0.1;
    const cv::Vec3b atmosphericLight = cv::Vec3b(255, 252, 249);
    std::unique_ptr<RemoveHaze> removeHaze(new RemoveHaze(minTransmission));
    removeHaze->execute(inputImage, transmissionMap, atmosphericLight, outputImage);

    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}

TEST(RemoveHazeTestCUDA, RemoveHazeCUDA) {
    // Read images
    const cv::Mat inputImage = cv::imread(inputImagePath);
    const cv::Mat transmissionMap = cv::imread(transmissionPath, cv::IMREAD_GRAYSCALE);
    const cv::Mat referenceImage = cv::imread(referenceImagePath);
    cv::Mat outputImage;

    cv::cuda::GpuMat dInputImage(inputImage);
    cv::cuda::GpuMat dTransmissionMap(transmissionMap);

    const size_t stepSize = dInputImage.step * sizeof(unsigned char);
    const size_t memSize  = stepSize * dInputImage.rows;

    float *outputImagePtr;
    cudaMalloc((void**)&outputImagePtr, memSize);
    cv::cuda::GpuMat dOutputImage(inputImage.rows, inputImage.cols, CV_8UC3, (void*)outputImagePtr, stepSize);

    dInputImage.upload(inputImage);
    dTransmissionMap.upload(transmissionMap);

    // Execute
    const double minTransmission = 0.1;
    const cv::Vec3b atmosphericLight = cv::Vec3b(255, 252, 249);
    std::unique_ptr<RemoveHaze> removeHaze(new RemoveHaze(minTransmission));
    removeHaze->execute(dInputImage, dTransmissionMap, atmosphericLight, dOutputImage);

    dOutputImage.download(outputImage);
    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}
