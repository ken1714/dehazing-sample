#include "dehazing.hpp"


Dehazing::Dehazing(const double candidateAreaRate, const int darkChannelNeighborRadius, const double minTransmission,
                   const double omega, const int guidedFilterRadius, const double guidedFilterEps) {
    // Initialization
    atmosphericLightEstimator = std::make_unique<AtmosphericLight>(candidateAreaRate);
    darkChannelPrior          = std::make_unique<DarkChannelPrior>(darkChannelNeighborRadius);
    hazeRemover               = std::make_unique<RemoveHaze>(minTransmission);
    transmissionMapEstimator  = std::make_unique<TransmissionMap>(omega, guidedFilterRadius, guidedFilterEps);
}


void Dehazing::execute(const cv::Mat& originalImage, cv::Mat& dehazedImage) {
    // Calculate dark channel image
    cv::Mat darkChannelImage;
    darkChannelPrior->execute(originalImage, darkChannelImage);

    // Estimate atmospheric light
    cv::Vec3b atmosphericLight;
    atmosphericLightEstimator->execute(originalImage, darkChannelImage, atmosphericLight);

    // Calculate transmission map
    cv::Mat transmissionMap;
    transmissionMapEstimator->execute(darkChannelImage, originalImage, atmosphericLight, transmissionMap);

    // Calculate dehazed image
    hazeRemover->execute(originalImage, transmissionMap, atmosphericLight, dehazedImage);
}
