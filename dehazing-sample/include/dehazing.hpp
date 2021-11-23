#pragma once

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "atmospheric_light.hpp"
#include "dark_channel_prior.hpp"
#include "remove_haze.hpp"
#include "transmission_map.hpp"

class Dehazing {
    public:
        Dehazing(const double candidateAreaRate, const int darkChannelNeighborRadius, const double minTransmission,
                 const double omega, const int guidedFilterRadius, const double guidedFilterEps);
        ~Dehazing();
        void execute(const cv::Mat& originalImage, cv::Mat& dehazedImage);

    protected:
        AtmosphericLight *atmosphericLightEstimator;
        DarkChannelPrior *darkChannelPrior;
        RemoveHaze *hazeRemover;
        TransmissionMap *transmissionMapEstimator;
};
