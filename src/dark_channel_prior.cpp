#include "dark_channel_prior.hpp"
#include "dark_channel_prior.cuh"


DarkChannelPrior::DarkChannelPrior(const int neighborRadius) {
    // Set erosion element (matrix's value must be 1)
    _darkChannelNeighbor = cv::Mat(cv::Size(neighborRadius, neighborRadius), CV_8U, cv::Scalar(1));
}


void DarkChannelPrior::execute(const cv::Mat& inputImage, cv::Mat& outputImage) {
    cv::Mat RGBMinimumImage;

    calcRGBMinimum(inputImage, RGBMinimumImage);

    calcDarkChannel(RGBMinimumImage, outputImage);
}


void DarkChannelPrior::calcRGBMinimum(const cv::Mat& inputImage, cv::Mat& RGBMinimumImage) {
    std::vector<cv::Mat> planes;

    // Split color images into B, G, R images
    cv::split(inputImage, planes);

    // Calculate RGB minimum image
    cv::Mat BGMinimum(inputImage.size(), CV_8UC1);
    cv::min(planes[0], planes[1], BGMinimum);
    cv::min(planes[2], BGMinimum, RGBMinimumImage);
}


void DarkChannelPrior::calcDarkChannel(const cv::Mat& RGBMinimumImage, cv::Mat& darkChannelImage) {
    cv::erode(RGBMinimumImage, darkChannelImage, _darkChannelNeighbor);
}


void DarkChannelPrior::execute(const cv::cuda::GpuMat& inputImage, cv::cuda::GpuMat& outputImage) {
    const int imageHeight = inputImage.rows;
    const int imageWidth  = inputImage.cols;
    cv::cuda::GpuMat RGBMinimumImage(inputImage.size(), CV_8UC1);

    executeDarkChannelPriorGPU(inputImage.data, RGBMinimumImage.data, outputImage.data, _darkChannelNeighbor.rows / 2, imageWidth, imageHeight);
}
