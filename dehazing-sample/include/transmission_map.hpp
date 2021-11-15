#pragma once

#include "image_process.hpp"

class TransmissionMap: public ImageProcess {
    public:
        TransmissionMap(const cv::Size imageSize, const cv::Mat& guideImage, const int guidedFilterRadius, const double guidedFilterEps);
        void execute(const cv::Mat& inputImage, cv::Mat& outputImage);

    protected:
        int _guidedFilterRadius;
        double _guidedFilterEps;
        cv::Mat _guideImage;
};
