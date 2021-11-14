#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class ImageProcess {
    public:
        ImageProcess(const cv::Size imageSize): _imageSize(imageSize) {};
        virtual ~ImageProcess() {};
        virtual void execute(const cv::Mat& inputImage, cv::Mat& outputImage) = 0;
    protected:
        cv::Size _imageSize;
};
