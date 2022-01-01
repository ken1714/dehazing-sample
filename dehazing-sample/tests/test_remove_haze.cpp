#include "gtest/gtest.h"
#include "remove_haze.hpp"
#include "utils.hpp"

TEST(RemoveHazeTest, RemoveHaze) {
    // Path
    const std::string inputImagePath = "data/original.png";
    const std::string transmissionPath = "data/transmission_map.png";
    const std::string referenceImagePath = "data/dehazed.png";


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
