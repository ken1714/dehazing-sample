#pragma once

#include "image_process.hpp"

class RemoveHaze: public ImageProcess {
    public:
        RemoveHaze(const cv::Size imageSize, const double minTransmission);
        void execute(const cv::Mat& inputImage, const cv::Mat& transmissionMap, const cv::Vec3b atmosphericLight, cv::Mat& outputImage);

    protected:
        double _minTransmission;
};
