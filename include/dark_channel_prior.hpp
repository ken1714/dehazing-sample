#pragma once

#include "image_process.hpp"

class DarkChannelPrior: public ImageProcess {
    public:
        DarkChannelPrior(const int neighborRadius);
        void execute(const cv::Mat& inputImage, cv::Mat& outputImage);

    protected:
        cv::Mat _darkChannelNeighbor;
        void calcRGBMinimum(const cv::Mat& inputImage, cv::Mat& RGBMinimumImage);
        void calcDarkChannel(const cv::Mat& RGBMinimumImage, cv::Mat& darkChannelImage);
};
