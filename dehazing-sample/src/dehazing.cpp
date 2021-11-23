#include "dehazing.hpp"


Dehazing::Dehazing(const double candidateAreaRate, const int darkChannelNeighborRadius, const double minTransmission,
                   const double omega, const int guidedFilterRadius, const double guidedFilterEps) {
    // Initialization
    atmosphericLightEstimator = new AtmosphericLight(candidateAreaRate);
    darkChannelPrior = new DarkChannelPrior(darkChannelNeighborRadius);
    hazeRemover = new RemoveHaze(minTransmission);
    transmissionMapEstimator = new TransmissionMap(omega, guidedFilterRadius, guidedFilterEps);
}


Dehazing::~Dehazing() {
    delete atmosphericLightEstimator;
    delete darkChannelPrior;
    delete hazeRemover;
    delete transmissionMapEstimator;
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
