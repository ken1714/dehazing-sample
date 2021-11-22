#pragma once

#include "image_process.hpp"

class TransmissionMap: public ImageProcess {
    public:
        TransmissionMap(const cv::Size imageSize, const double omega, const int guidedFilterRadius, const double guidedFilterEps);
        void execute(const cv::Mat& inputImage, const cv::Mat& guideImage, const cv::Vec3b atmosphericLight, cv::Mat& outputImage);

    protected:
        double _omega;
        int _guidedFilterRadius;
        double _guidedFilterEps;
        void calcRoughTransmission(const cv::Mat& darkChannelImage, const cv::Vec3b atmosphericLight, cv::Mat& roughTransmission);
};
