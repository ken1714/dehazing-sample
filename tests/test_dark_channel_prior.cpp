#include "gtest/gtest.h"
#include "dark_channel_prior.hpp"
#include "dark_channel_prior.cuh"
#include "utils.hpp"


// Path
const std::string inputImagePath = "data/original.png";
const std::string referenceImagePath = "data/dark_channel.png";


TEST(DarkChannelTest, DarkChannelPrior) {
    // Read images
    const cv::Mat inputImage = cv::imread(inputImagePath);
    const cv::Mat referenceImage = cv::imread(referenceImagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat outputImage;

    // Execute
    const int neighborSize = 15;
    std::unique_ptr<DarkChannelPrior> darkChannelPrior(new DarkChannelPrior(neighborSize));
    darkChannelPrior->execute(inputImage, outputImage);

    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}


TEST(DarkChannelTestCUDA, DarkChannelPriorCUDA) {
    // Read images
    const cv::Mat inputImage = cv::imread(inputImagePath);
    const cv::Mat referenceImage = cv::imread(referenceImagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat outputImage;

    cv::cuda::GpuMat dInputImage(inputImage);
    dInputImage.upload(inputImage);

    cv::cuda::GpuMat dOutputImage(inputImage.rows, inputImage.cols, CV_8UC1);

    // Execute
    const int neighborSize = 15;
    std::unique_ptr<DarkChannelPrior> darkChannelPrior(new DarkChannelPrior(neighborSize));
    darkChannelPrior->execute(dInputImage, dOutputImage);

    dOutputImage.download(outputImage);
    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}
