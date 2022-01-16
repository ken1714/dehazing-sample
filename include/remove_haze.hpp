#pragma once

#include "image_process.hpp"

class RemoveHaze: public ImageProcess {
    public:
        RemoveHaze(const double minTransmission);
        void execute(const cv::Mat& inputImage, const cv::Mat& transmissionMap, const cv::Vec3b atmosphericLight, cv::Mat& outputImage);
        void execute(const cv::cuda::GpuMat& inputImage, const cv::cuda::GpuMat& transmissionMap, const cv::Vec3b atmosphericLight, cv::cuda::GpuMat& outputImage);

    protected:
        double _minTransmission;
};
