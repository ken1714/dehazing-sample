#include "gtest/gtest.h"
#include "dark_channel_prior.hpp"
#include "utils.hpp"

TEST(DarkChannelTest, DarkChannelPrior) {
    // Path
    const std::string inputImagePath = "data/original.png";
    const std::string referenceImagePath = "data/dark_channel.png";

    // Read images
    const cv::Mat inputImage = cv::imread(inputImagePath);
    const cv::Mat referenceImage = cv::imread(referenceImagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat outputImage;

    // Execute
    const int neighborRadius = 15;
    DarkChannelPrior darkChannelPrior(inputImage.size(), neighborRadius);
    darkChannelPrior.execute(inputImage, outputImage);

    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}
