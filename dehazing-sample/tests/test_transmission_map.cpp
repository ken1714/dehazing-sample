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
    const int guidedFilterRadius = 30;
    const double guidedFilterEps = 30;
    TransmissionMap *transmissionMap = new TransmissionMap(inputImage.size(), guideImage, guidedFilterRadius, guidedFilterEps);
    transmissionMap->execute(inputImage, outputImage);

    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}
