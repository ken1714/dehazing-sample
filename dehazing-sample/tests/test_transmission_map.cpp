#include "gtest/gtest.h"
#include "transmission_map.hpp"
#include "utils.hpp"

TEST(TransmissionMapTest, TransmissionMap) {
    // Path
    const std::string guidedImagePath = "data/original.png";
    const std::string inputImagePath = "data/dark_channel.png";
    const std::string referenceImagePath = "data/transmission_map.png";

    // Read images
    const cv::Mat guideImage = cv::imread(guidedImagePath);
    const cv::Mat inputImage = cv::imread(inputImagePath, cv::IMREAD_GRAYSCALE);
    const cv::Mat referenceImage = cv::imread(referenceImagePath, cv::IMREAD_GRAYSCALE);
    cv::Mat outputImage;

    // Execute
    const double omega = 0.95;
    const int guidedFilterRadius = 30;
    const double guidedFilterEps = 30;
    const cv::Vec3b atmosphericLight = cv::Vec3b(255, 252, 249);
    std::unique_ptr<TransmissionMap> transmissionMap(new TransmissionMap(omega, guidedFilterRadius, guidedFilterEps));
    transmissionMap->execute(inputImage, guideImage, atmosphericLight, outputImage);

    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}
