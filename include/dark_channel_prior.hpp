#pragma once

#include "image_process.hpp"

class DarkChannelPrior: public ImageProcess {
    public:
        DarkChannelPrior(const int neighborSize);
        void execute(const cv::Mat& inputImage, cv::Mat& outputImage);
        void execute(const cv::cuda::GpuMat& inputImage, cv::cuda::GpuMat& outputImage);

    protected:
        int _neighborRadius;
        cv::Mat _darkChannelNeighbor;
        void calcRGBMinimum(const cv::Mat& inputImage, cv::Mat& RGBMinimumImage);
        void calcDarkChannel(const cv::Mat& RGBMinimumImage, cv::Mat& darkChannelImage);
};
