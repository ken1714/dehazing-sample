#include "transmission_map.hpp"
#include <opencv2/ximgproc/edge_filter.hpp>


TransmissionMap::TransmissionMap(const cv::Size imageSize, const cv::Mat& guideImage, const int guidedFilterRadius, const double guidedFilterEps):
    ImageProcess(imageSize), _guidedFilterRadius(guidedFilterRadius), _guidedFilterEps(guidedFilterEps), _guideImage(guideImage) {
}

void TransmissionMap::execute(const cv::Mat& inputImage, cv::Mat& outputImage) {
    // Fit Dark Channel image to guided image (original image)
    // _guidedFilterEps is regularization term
    cv::ximgproc::guidedFilter(_guideImage, inputImage, outputImage, _guidedFilterRadius, _guidedFilterEps);
}
