#include "transmission_map.hpp"
#include <opencv2/ximgproc/edge_filter.hpp>


TransmissionMap::TransmissionMap(const double omega, const int guidedFilterRadius, const double guidedFilterEps):
    _omega(omega), _guidedFilterRadius(guidedFilterRadius), _guidedFilterEps(guidedFilterEps) {
    if (omega < 0 || omega > 1.0) {
        std::cerr << "Invalid omega for calculating rough transmission. Please set [0, 1] value." << std::endl;
    }
}


void TransmissionMap::execute(const cv::Mat& inputImage, const cv::Mat& guideImage, const cv::Vec3b atmosphericLight, cv::Mat& outputImage) {
    // Calculate rough transmission map using dark channel image.
    cv::Mat roughTransmission;
    calcRoughTransmission(inputImage, atmosphericLight, roughTransmission);

    // Fit Dark Channel image to guided image (original image)
    // _guidedFilterEps is regularization term
    cv::ximgproc::guidedFilter(guideImage, roughTransmission, outputImage, _guidedFilterRadius, _guidedFilterEps);
}


void TransmissionMap::calcRoughTransmission(const cv::Mat& darkChannelImage, const cv::Vec3b atmosphericLight, cv::Mat& roughTransmission) {
    // The highest channel of atmospheric light color
    const double maxAtmosphericLight = (double)(unsigned char)std::max({atmosphericLight[0], atmosphericLight[1], atmosphericLight[2]});

    cv::Mat darkChannelDouble;
    darkChannelImage.convertTo(darkChannelDouble, CV_64F);

    // t = 1 - omega * min( min( I / A ) )
    cv::Mat roughTransmissionDouble;
    roughTransmissionDouble = (1.0 - _omega * darkChannelDouble / maxAtmosphericLight) * 255;
    roughTransmissionDouble.convertTo(roughTransmission, CV_8U);
}
