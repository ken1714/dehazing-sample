#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class AtmosphericLight {
    public:
        AtmosphericLight(const double candidateAreaRate);
        void execute(const cv::Mat& originalImage, const cv::Mat& darkChannelImage, cv::Vec3b& atmosphericLight);
        cv::Mat getCandidateAreaImage();

    protected:
        int _candidateAreaThreshold;
        double _candidateAreaRate;
        cv::Mat _candidateAreaImage;
        int calculateCandidateAreaLowerbound(const cv::Mat& darkChannelImage);
        cv::Vec3b extractAtmosphericLight(const cv::Mat& originalImage, const cv::Mat& darkChannelImage, const int candidateAreaThreshold);
        void drawCandidateArea(const cv::Mat& originalImage, const cv::Mat& darkChannelImage, const int candidateAreaThreshold);
};
