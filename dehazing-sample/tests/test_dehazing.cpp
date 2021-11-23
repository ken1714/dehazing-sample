#include "gtest/gtest.h"
#include "dehazing.hpp"
#include "utils.hpp"

TEST(DehazingTest, Dehazing) {
    // Path
    const std::string inputImagePath = "data/original.png";
    const std::string referenceImagePath = "data/dehazed.png";


    // Read images
    const cv::Mat inputImage = cv::imread(inputImagePath);
    const cv::Mat referenceImage = cv::imread(referenceImagePath);
    cv::Mat outputImage;

    // Parameters
    const double candidateAreaRate = 0.001;
    const int darkChannelNeighborRadius = 15;
    const double minTransmission = 0.1;
    const double omega = 0.95;
    const int guidedFilterRadius = 30;
    const double guidedFilterEps = 30;

    // Execute
    Dehazing *dehazing = new Dehazing(candidateAreaRate, darkChannelNeighborRadius, minTransmission, omega, guidedFilterRadius, guidedFilterEps);
    dehazing->execute(inputImage, outputImage);

    EXPECT_EQ(equal2Images(outputImage, referenceImage), true);
}
