#include "gtest/gtest.h"
#include "dark_channel_prior.hpp"
#include "utils.hpp"

TEST(DarkChannelTest, DarkChannelPrior) {
    cv::Mat inputImage = cv::imread("data/original.png");
    cv::Mat referenceImage = cv::imread("data/dark_channel.png", cv::IMREAD_GRAYSCALE);
    cv::Mat outputImage;

    DarkChannelPrior darkChannelPrior(inputImage.size(), 15);
    darkChannelPrior.execute(inputImage, outputImage);

    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}
