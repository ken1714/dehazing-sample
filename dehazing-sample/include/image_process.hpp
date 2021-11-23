#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class ImageProcess {
    public:
        ImageProcess(const cv::Size imageSize): _imageSize(imageSize) {};
        virtual ~ImageProcess() {};
        void execute(const cv::Mat& inputImage, cv::Mat& outputImage);
    protected:
        cv::Size _imageSize;
};
