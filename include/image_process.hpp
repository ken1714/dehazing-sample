#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class ImageProcess {
    public:
        ImageProcess() {};
        virtual ~ImageProcess() {};
        void execute(const cv::Mat& inputImage, cv::Mat& outputImage);
};
