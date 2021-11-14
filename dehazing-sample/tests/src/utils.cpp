#include "utils.hpp"

bool equal2Images(const cv::Mat& image1, const cv::Mat& image2) {
    cv::Mat equalFlag;
    cv::compare(image1, image2, equalFlag, cv::CMP_EQ);

    double minValue, maxValue;
    cv::minMaxLoc(equalFlag, &minValue, &maxValue);

    if (fabs(minValue - maxValue) < DBL_EPSILON) {
        return true;
    } else {
        return false;
    }
}