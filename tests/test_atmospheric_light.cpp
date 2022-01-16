#include "gtest/gtest.h"
#include "atmospheric_light.hpp"

TEST(AtmosphericLightTest, AtmosphericLight) {
    // Path
    const std::string inputImagePath = "data/original.png";
    const std::string darkChannelImagePath = "data/dark_channel.png";

    // Read images
    const cv::Mat inputImage = cv::imread(inputImagePath);
    const cv::Mat darkChannelImage = cv::imread(darkChannelImagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat outputImage;

    // Execute
    const double candidateAreaRate = 0.001;
    cv::Vec3b atmosphericLight;
    std::unique_ptr<AtmosphericLight> atmosphericLightEstimator(new AtmosphericLight(candidateAreaRate));
    atmosphericLightEstimator->execute(inputImage, darkChannelImage, atmosphericLight);

    EXPECT_EQ((int)(unsigned char)atmosphericLight[0], 255);
    EXPECT_EQ((int)(unsigned char)atmosphericLight[1], 252);
    EXPECT_EQ((int)(unsigned char)atmosphericLight[2], 249);
}
